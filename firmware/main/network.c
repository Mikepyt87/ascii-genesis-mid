// SPDX-License-Identifier: GPL-3.0-or-later
#include "network.h"
#include "portal_access.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lwip/sockets.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "esp_netif.h"
#include "esp_random.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "nvs.h"

static SemaphoreHandle_t lock;
static QueueHandle_t commands;
static net_status_t status;
static httpd_handle_t portal;
static char ssid[33], password[65], pending_ssid[33], pending_password[65], token[17];
static bool credentials_pending;
static int64_t portal_until;

static void message(const char *s) {xSemaphoreTake(lock,portMAX_DELAY);strlcpy(status.message,s,sizeof(status.message));xSemaphoreGive(lock);}
void network_status(net_status_t *out) {xSemaphoreTake(lock,portMAX_DELAY);*out=status;xSemaphoreGive(lock);}
void network_command(net_command_t cmd) {xQueueSend(commands,&cmd,0);}
static void wifi_event(void *arg,esp_event_base_t base,int32_t id,void *data) {
    (void)arg;(void)data;
    xSemaphoreTake(lock,portMAX_DELAY);
    if(base==IP_EVENT && id==IP_EVENT_STA_GOT_IP)status.online=true;
    if(base==WIFI_EVENT && id==WIFI_EVENT_STA_DISCONNECTED)status.online=false;
    xSemaphoreGive(lock);
}
static void connect_saved(void) {
    if(!ssid[0]) {message("Offline aquarium - open Settings for Wi-Fi");return;}
    wifi_config_t cfg={0};
    strlcpy((char*)cfg.sta.ssid,ssid,sizeof(cfg.sta.ssid));
    strlcpy((char*)cfg.sta.password,password,sizeof(cfg.sta.password));
    cfg.sta.pmf_cfg.capable=true;
    esp_wifi_disconnect();
    if(esp_wifi_set_config(WIFI_IF_STA,&cfg)==ESP_OK)esp_wifi_connect();
}
// Decodes a form field without accepting truncation or embedded NUL bytes.
static bool decode(const char *src,char *dst,size_t cap) {
    size_t n=0;
    for(;*src;++src) {
        unsigned char c=*src;
        if(c=='+') c=' ';
        else if(c=='%') {
            if(!src[1]||!src[2])return false;
            char hex[3]={src[1],src[2],0},*end; long value=strtol(hex,&end,16);
            if(*end||value==0)return false;
            c=(unsigned char)value;src+=2;
        }
        if(c<32||n+1>=cap)return false;
        dst[n++]=(char)c;
    }
    dst[n]=0;return true;
}
static bool portal_on_setup_network(httpd_req_t *req) {
    struct sockaddr_storage local = {0};socklen_t len=sizeof(local);
    esp_netif_ip_info_t ap_ip;
    esp_netif_t *ap=esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    if(!ap || esp_netif_get_ip_info(ap,&ap_ip)!=ESP_OK)return false;
    if(getsockname(httpd_req_to_sockfd(req),(struct sockaddr*)&local,&len)!=0)return false;
    return portal_address_matches(&local,len,ap_ip.ip.addr);
}
static esp_err_t portal_get(httpd_req_t *req) {
    if(!portal_on_setup_network(req))return httpd_resp_send_err(req,HTTPD_403_FORBIDDEN,"Join the device setup network first");
    char page[1800];
    snprintf(page,sizeof(page),
      "<!doctype html><meta name='viewport' content='width=device-width,initial-scale=1'>"
      "<title>ASCII Genesis Mid Wi-Fi</title><style>body{font:20px system-ui;max-width:28em;margin:2em auto;padding:1em}"
      "label,input,button{display:block;margin:1em 0;width:100%%;font:inherit;box-sizing:border-box;padding:.5em}</style>"
      "<h1>Connect your aquarium</h1><p>Choose your 2.4 GHz home network. The aquarium also works offline.</p>"
      "<form method='post' action='/wifi'><input type='hidden' name='token' value='%s'>"
      "<label>Wi-Fi name<input name='ssid' maxlength='32' required autocomplete='off'></label>"
      "<label>Wi-Fi password<input name='password' type='password' maxlength='64' autocomplete='off'></label>"
      "<button>Save and connect</button></form>",token);
    httpd_resp_set_hdr(req,"Cache-Control","no-store");
    httpd_resp_set_hdr(req,"Content-Security-Policy","default-src 'none'; style-src 'unsafe-inline'; form-action 'self'; frame-ancestors 'none'");
    return httpd_resp_sendstr(req,page);
}
static esp_err_t portal_post(httpd_req_t *req) {
    if(!portal_on_setup_network(req))return httpd_resp_send_err(req,HTTPD_403_FORBIDDEN,"Join the device setup network first");
    if(req->content_len<=0||req->content_len>512)return httpd_resp_send_err(req,HTTPD_400_BAD_REQUEST,"Invalid form");
    char body[513],raw[256],name[33],pass[65],submitted_token[17];int used=0;
    while(used<req->content_len) {int n=httpd_req_recv(req,body+used,req->content_len-used);if(n<=0)return ESP_FAIL;used+=n;}
    body[used]=0;
    if(httpd_query_key_value(body,"token",submitted_token,sizeof(submitted_token))!=ESP_OK || strcmp(token,submitted_token)
       || httpd_query_key_value(body,"ssid",raw,sizeof(raw))!=ESP_OK || !decode(raw,name,sizeof(name)) || !name[0])
        return httpd_resp_send_err(req,HTTPD_400_BAD_REQUEST,"Invalid setup form");
    if(httpd_query_key_value(body,"password",raw,sizeof(raw))!=ESP_OK || !decode(raw,pass,sizeof(pass)))
        return httpd_resp_send_err(req,HTTPD_400_BAD_REQUEST,"Invalid password");
    size_t len=strlen(pass);
    if(len && (len<8 || (len>63 && len!=64)))return httpd_resp_send_err(req,HTTPD_400_BAD_REQUEST,"Password must be 8-63 characters, or empty for an open network");
    if(len==64) for(int i=0;i<64;i++) if(!((pass[i]>='0'&&pass[i]<='9')||(pass[i]>='a'&&pass[i]<='f')||(pass[i]>='A'&&pass[i]<='F')))
        return httpd_resp_send_err(req,HTTPD_400_BAD_REQUEST,"A 64-character key must be hexadecimal");
    xSemaphoreTake(lock,portMAX_DELAY);
    strlcpy(pending_ssid,name,sizeof(pending_ssid));strlcpy(pending_password,pass,sizeof(pending_password));credentials_pending=true;
    xSemaphoreGive(lock);
    httpd_resp_set_hdr(req,"Cache-Control","no-store");
    return httpd_resp_sendstr(req,"Saved for connection. Rejoin your usual Wi-Fi and check the aquarium screen. If connection fails, reopen Wi-Fi setup on the aquarium.");
}
static void close_portal(void) {
    if(portal){httpd_stop(portal);portal=NULL;}
    esp_wifi_set_mode(WIFI_MODE_STA);
    xSemaphoreTake(lock,portMAX_DELAY);status.setup=false;memset(status.ap_password,0,sizeof(status.ap_password));xSemaphoreGive(lock);
    memset(token,0,sizeof(token));
}
static void open_portal(void) {
    close_portal();
    wifi_config_t ap={0};uint32_t random=esp_random();
    char ap_name[33],ap_pass[17];
    snprintf(ap_name,sizeof(ap_name),"ASCII-Genesis-Mid-%04lx",(unsigned long)(random&65535));
    snprintf(ap_pass,sizeof(ap_pass),"%08lx%08lx",(unsigned long)esp_random(),(unsigned long)esp_random());
    snprintf(token,sizeof(token),"%08lx%08lx",(unsigned long)esp_random(),(unsigned long)esp_random());
    strlcpy((char*)ap.ap.ssid,ap_name,sizeof(ap.ap.ssid));strlcpy((char*)ap.ap.password,ap_pass,sizeof(ap.ap.password));
    ap.ap.authmode=WIFI_AUTH_WPA2_PSK;ap.ap.max_connection=1;
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP,&ap));
    httpd_config_t cfg=HTTPD_DEFAULT_CONFIG();cfg.stack_size=6144;cfg.recv_wait_timeout=5;cfg.lru_purge_enable=true;
    if(httpd_start(&portal,&cfg)!=ESP_OK){close_portal();message("Wi-Fi setup could not start");return;}
    httpd_uri_t get={.uri="/",.method=HTTP_GET,.handler=portal_get};
    httpd_uri_t post={.uri="/wifi",.method=HTTP_POST,.handler=portal_post};
    ESP_ERROR_CHECK(httpd_register_uri_handler(portal,&get));ESP_ERROR_CHECK(httpd_register_uri_handler(portal,&post));
    xSemaphoreTake(lock,portMAX_DELAY);status.setup=true;strlcpy(status.ap_name,ap_name,sizeof(status.ap_name));strlcpy(status.ap_password,ap_pass,sizeof(status.ap_password));xSemaphoreGive(lock);
    portal_until=esp_timer_get_time()+600000000LL;message("Phone: join setup Wi-Fi, open 192.168.4.1");
}
static void check_update(void) {message("USB updates only - no published channel yet");}
static void install_update(void) {check_update();}
static void network_task(void *arg) {
    (void)arg;connect_saved();int64_t next_connect=0;
    while(true) {
        net_command_t cmd;
        if(xQueueReceive(commands,&cmd,pdMS_TO_TICKS(250))) {
            switch(cmd){case NET_SETUP:open_portal();break;case NET_CLOSE_SETUP:close_portal();break;case NET_CHECK:check_update();break;case NET_INSTALL:install_update();break;}
        }
        bool pending;
        xSemaphoreTake(lock,portMAX_DELAY);pending=credentials_pending;credentials_pending=false;
        if(pending){strlcpy(ssid,pending_ssid,sizeof(ssid));strlcpy(password,pending_password,sizeof(password));memset(pending_password,0,sizeof(pending_password));}
        xSemaphoreGive(lock);
        if(pending) {
            nvs_handle_t nvs;esp_err_t err=nvs_open("wifi4848",NVS_READWRITE,&nvs);
            if(err==ESP_OK){err=nvs_set_str(nvs,"ssid",ssid);if(err==ESP_OK)err=nvs_set_str(nvs,"password",password);if(err==ESP_OK)err=nvs_commit(nvs);nvs_close(nvs);}
            if(err==ESP_OK){vTaskDelay(pdMS_TO_TICKS(1500));close_portal();message("Connecting to home Wi-Fi...");connect_saved();}
            else message("Could not save Wi-Fi - try setup again");
        }
        int64_t now=esp_timer_get_time();net_status_t s;network_status(&s);
        if(s.setup&&now>portal_until){close_portal();message("Setup timed out - aquarium stays offline-ready");}
        if(!s.online&&ssid[0]&&now>next_connect){esp_wifi_connect();next_connect=now+15000000LL;}
    }
}
void network_start(void) {
    lock=xSemaphoreCreateMutex();commands=xQueueCreate(4,sizeof(net_command_t));assert(lock&&commands);
    message("Offline aquarium ready");
    nvs_handle_t nvs;if(nvs_open("wifi4848",NVS_READONLY,&nvs)==ESP_OK){size_t n=sizeof(ssid);nvs_get_str(nvs,"ssid",ssid,&n);n=sizeof(password);nvs_get_str(nvs,"password",password,&n);nvs_close(nvs);}
    ESP_ERROR_CHECK(esp_netif_init());ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg=WIFI_INIT_CONFIG_DEFAULT();ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,ESP_EVENT_ANY_ID,wifi_event,NULL));ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,IP_EVENT_STA_GOT_IP,wifi_event,NULL));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));ESP_ERROR_CHECK(esp_wifi_start());
    if(xTaskCreate(network_task,"wifi_updates",16384,NULL,3,NULL)!=pdPASS)ESP_ERROR_CHECK(ESP_ERR_NO_MEM);
}
