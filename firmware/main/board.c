// SPDX-License-Identifier: GPL-3.0-or-later
#include "board.h"
#include "touch_packet.h"
#include <assert.h>
#include <string.h>
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

enum { CS=39, SCK=48, MOSI=47, BACKLIGHT=38 };
static esp_lcd_panel_handle_t panel;
static i2c_master_dev_handle_t touch;
static uint16_t *scaled;
// ST7701 9-bit serial command interface. Pixel data uses the separate RGB bus.
static void serial_word(bool data,uint8_t value) {
    gpio_set_level(CS,0);
    unsigned word=(data?0x100:0)|value;
    for(int bit=8;bit>=0;--bit){
        gpio_set_level(SCK,0);gpio_set_level(MOSI,(word>>bit)&1);esp_rom_delay_us(1);
        gpio_set_level(SCK,1);esp_rom_delay_us(1);
    }
    gpio_set_level(SCK,0);gpio_set_level(CS,1);
}
typedef struct {uint8_t command,size,data[16];} panel_command_t;
// Guition 4848 / ST7701 type 9 register values. Source and attribution in HARDWARE.md.
static const panel_command_t init[] = {
    {0xFF,5,{0x77,1,0,0,0x10}},
    {0xC0,2,{0x3B,0}},{0xC1,2,{0x0D,2}},{0xC2,2,{0x31,5}},{0xCD,1,{0}},
    {0xB0,16,{0,0x11,0x18,0x0E,0x11,6,7,8,7,0x22,4,0x12,0x0F,0xAA,0x31,0x18}},
    {0xB1,16,{0,0x11,0x19,0x0E,0x12,7,8,8,8,0x22,4,0x11,0x11,0xA9,0x32,0x18}},
    {0xFF,5,{0x77,1,0,0,0x11}},
    {0xB0,1,{0x60}},{0xB1,1,{0x32}},{0xB2,1,{7}},{0xB3,1,{0x80}},
    {0xB5,1,{0x49}},{0xB7,1,{0x85}},{0xB8,1,{0x21}},{0xC1,1,{0x78}},{0xC2,1,{0x78}},
    {0xE0,3,{0,0x1B,2}},{0xE1,11,{8,0xA0,0,0,7,0xA0,0,0,0,0x44,0x44}},
    {0xE2,12,{0x11,0x11,0x44,0x44,0xED,0xA0,0,0,0xEC,0xA0,0,0}},
    {0xE3,4,{0,0,0x11,0x11}},{0xE4,2,{0x44,0x44}},
    {0xE5,16,{0x0A,0xE9,0xD8,0xA0,0x0C,0xEB,0xD8,0xA0,0x0E,0xED,0xD8,0xA0,0x10,0xEF,0xD8,0xA0}},
    {0xE6,4,{0,0,0x11,0x11}},{0xE7,2,{0x44,0x44}},
    {0xE8,16,{9,0xE8,0xD8,0xA0,0x0B,0xEA,0xD8,0xA0,0x0D,0xEC,0xD8,0xA0,0x0F,0xEE,0xD8,0xA0}},
    {0xEB,7,{2,0,0xE4,0xE4,0x88,0,0x40}},{0xEC,2,{0x3C,0}},
    {0xED,16,{0xAB,0x89,0x76,0x54,2,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x20,0x45,0x67,0x98,0xBA}},
    {0xFF,5,{0x77,1,0,0,0x13}},{0xE5,1,{0xE4}},
    {0xFF,5,{0x77,1,0,0,0}},{0x36,1,{0}},{0x3A,1,{0x60}}
};
static esp_err_t touch_read(uint16_t reg,void *buf,size_t len) {
    uint8_t address[2];guition_touch_register(reg,address);
    return i2c_master_transmit_receive(touch,address,2,buf,len,100);
}
static esp_err_t touch_ack(void) {
    uint8_t clear[3]={0};guition_touch_register(0x814E,clear);
    return i2c_master_transmit(touch,clear,sizeof(clear),100);
}
void board_init(void) {
    gpio_config_t pins={.pin_bit_mask=(1ULL<<CS)|(1ULL<<SCK)|(1ULL<<MOSI)|(1ULL<<BACKLIGHT),.mode=GPIO_MODE_OUTPUT};
    ESP_ERROR_CHECK(gpio_config(&pins));gpio_set_level(BACKLIGHT,0);gpio_set_level(CS,1);
    serial_word(false,0x01);vTaskDelay(pdMS_TO_TICKS(150));
    for(unsigned c=0;c<sizeof(init)/sizeof(init[0]);++c){
        serial_word(false,init[c].command);
        for(unsigned d=0;d<init[c].size;++d)serial_word(true,init[c].data[d]);
    }
    serial_word(false,0x11);vTaskDelay(pdMS_TO_TICKS(120));serial_word(false,0x29);
    esp_lcd_rgb_panel_config_t rgb={
        .clk_src=LCD_CLK_SRC_DEFAULT,
        .timings={.pclk_hz=10000000,.h_res=480,.v_res=480,
            .hsync_pulse_width=8,.hsync_back_porch=50,.hsync_front_porch=10,
            .vsync_pulse_width=8,.vsync_back_porch=20,.vsync_front_porch=10},
        .data_width=16,.in_color_format=LCD_COLOR_FMT_RGB565,.out_color_format=LCD_COLOR_FMT_RGB565,
        .num_fbs=1,.bounce_buffer_size_px=480*10,.dma_burst_size=64,
        .hsync_gpio_num=16,.vsync_gpio_num=17,.de_gpio_num=18,.pclk_gpio_num=21,.disp_gpio_num=-1,
        // RGB565 bit order: B0..B4, G0..G5, R0..R4.
        .data_gpio_nums={4,5,6,7,15,8,20,3,46,9,10,11,12,13,14,0},
        .flags.fb_in_psram=true
    };
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&rgb,&panel));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel));ESP_ERROR_CHECK(esp_lcd_panel_init(panel));
    scaled=heap_caps_calloc(480*480,sizeof(uint16_t),MALLOC_CAP_SPIRAM);assert(scaled);
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel,0,0,480,480,scaled));
    gpio_set_level(BACKLIGHT,1);
    i2c_master_bus_config_t bus={.i2c_port=I2C_NUM_0,.sda_io_num=19,.scl_io_num=45,
        .clk_source=I2C_CLK_SRC_DEFAULT,.glitch_ignore_cnt=7,.flags.enable_internal_pullup=true};
    i2c_master_bus_handle_t handle;ESP_ERROR_CHECK(i2c_new_master_bus(&bus,&handle));
    int address=i2c_master_probe(handle,0x5D,100)==ESP_OK?0x5D:0x14;
    ESP_ERROR_CHECK(i2c_master_probe(handle,address,100));
    i2c_device_config_t dev={.dev_addr_length=I2C_ADDR_BIT_LEN_7,.device_address=address,.scl_speed_hz=400000};
    ESP_ERROR_CHECK(i2c_master_bus_add_device(handle,&dev,&touch));
    uint8_t product[5]={0};ESP_ERROR_CHECK(touch_read(0x8140,product,4));
    ESP_LOGI("board","GUITION 4848S040: 480x480 RGB, touch %s at 0x%02X (ID %02X %02X %02X %02X)",product,address,product[0],product[1],product[2],product[3]);
    uint8_t resolution[4];ESP_ERROR_CHECK(touch_read(0x8048,resolution,4));
    ESP_LOGI("touch","GT911 configured range: %u x %u",resolution[0]|resolution[1]<<8,resolution[2]|resolution[3]<<8);
}
void board_frame(const uint16_t *pixels) {
    // Uniform 3:2 enlargement: the square ocean keeps its original proportions.
    for(int y=0;y<480;++y)for(int x=0;x<480;++x)scaled[y*480+x]=pixels[(y*2/3)*320+x*2/3];
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel,0,0,480,480,scaled));
}
bool board_touch(int *x,int *y) {
    // GT911 retains its previous state when no new packet is ready.
    static bool pressed;
    static int last_x,last_y;
    uint8_t status;
    if(touch_read(0x814E,&status,1)!=ESP_OK){pressed=false;return false;}
    if(status&0x80){
        bool was_pressed=pressed;
        uint8_t point[8]={0};
        if((status&15)>0&&touch_read(0x8150,point,sizeof(point))!=ESP_OK){pressed=false;return false;}
        if(touch_ack()!=ESP_OK){pressed=false;return false;}
        pressed=guition_touch_decode(status,point,sizeof(point),&last_x,&last_y);
        if(pressed&&!was_pressed)ESP_LOGI("touch","Press x=%d y=%d",last_x,last_y);
    }
    if(pressed){*x=last_x;*y=last_y;}
    return pressed;
}
