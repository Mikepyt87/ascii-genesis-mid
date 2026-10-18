// SPDX-License-Identifier: GPL-3.0-or-later
#include "../firmware/main/touch_packet.h"
// No runtime dependencies: this same test runs natively in CI or as WebAssembly.
int touch_check(void) {
    uint8_t reg[2];guition_touch_register(0x814E,reg);if(reg[0]!=0x81||reg[1]!=0x4E)return 10;
    guition_touch_register(0x8150,reg);if(reg[0]!=0x81||reg[1]!=0x50)return 11;
    guition_touch_register(0x8140,reg);if(reg[0]!=0x81||reg[1]!=0x40)return 12;
    int x=-1,y=-1;
    uint8_t point[8]={0};
    if(!guition_touch_decode(0x81,point,8,&x,&y)||x!=0||y!=0)return 1;
    point[0]=0xDF;point[1]=1;point[2]=0xDF;point[3]=1;
    if(!guition_touch_decode(0x81,point,8,&x,&y)||x!=319||y!=319)return 2;
    point[0]=240;point[1]=0;point[2]=196;point[3]=1;
    if(!guition_touch_decode(0x81,point,8,&x,&y)||x!=160||y!=301)return 3;
    if(guition_touch_decode(0x80,point,8,&x,&y))return 4;
    if(guition_touch_decode(0x01,point,8,&x,&y))return 5;
    if(guition_touch_decode(0x86,point,8,&x,&y))return 6;
    if(guition_touch_decode(0x81,point,7,&x,&y))return 7;
    point[0]=0xE0;point[1]=1;
    if(guition_touch_decode(0x81,point,8,&x,&y))return 8;
    // Every physical pixel maps into the logical framebuffer.
    for(unsigned py=0;py<480;py++)for(unsigned px=0;px<480;px++){
        point[0]=px;point[1]=px>>8;point[2]=py;point[3]=py>>8;
        if(!guition_touch_decode(0x81,point,8,&x,&y)||x!=(int)(px*2/3)||y!=(int)(py*2/3))return 9;
    }
    return 0;
}
#ifndef WASM_CHECK
int main(void){return touch_check();}
#endif
