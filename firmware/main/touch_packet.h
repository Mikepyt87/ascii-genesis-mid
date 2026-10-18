// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
// GT911 register addresses are high byte first; coordinates are low byte first.
static inline void guition_touch_register(uint16_t reg,uint8_t out[2]) {out[0]=reg>>8;out[1]=reg&255;}
static inline bool guition_touch_decode(uint8_t status,const uint8_t *p,size_t size,int *x,int *y) {
    if(!(status&0x80)||(status&15)==0||(status&15)>5||size<8)return false;
    unsigned raw_x=p[0]|(p[1]<<8),raw_y=p[2]|(p[3]<<8);
    if(raw_x>=480||raw_y>=480)return false;
    *x=raw_x*2/3;*y=raw_y*2/3;
    return true;
}
