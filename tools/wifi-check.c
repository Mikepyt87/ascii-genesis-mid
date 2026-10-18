// SPDX-License-Identifier: GPL-3.0-or-later
#include <assert.h>
#include <string.h>
#include "../firmware/main/wifi_status.h"
int main(void){
    assert(!wifi_link_retry_due(LINK_CONNECTING,100,0));
    assert(!wifi_link_retry_due(LINK_CONNECTED,100,0));
    assert(!wifi_link_retry_due(LINK_AUTH_FAILED,99,100));
    assert(wifi_link_retry_due(LINK_AUTH_FAILED,100,100));
    assert(!wifi_link_timed_out(LINK_CONNECTED,100,50));
    assert(!wifi_link_timed_out(LINK_CONNECTING,49,50));
    assert(wifi_link_timed_out(LINK_CONNECTING,50,50));
    assert(!strcmp(wifi_link_message(LINK_CONNECTED),"Connected to home Wi-Fi"));
    assert(strstr(wifi_link_message(LINK_AUTH_FAILED),"password"));
    assert(strstr(wifi_link_message(LINK_NOT_FOUND),"2.4 GHz"));
    assert(strstr(wifi_link_message(LINK_SECURITY_FAILED),"WPA2"));
    for(int s=LINK_OFFLINE;s<=LINK_TIMEOUT;s++)assert(strlen(wifi_link_message(s))<=46);
}
