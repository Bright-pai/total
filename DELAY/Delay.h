#ifndef _DELAY_H
#define _DELAY_H

#include "ti_msp_dl_config.h"
#include <stdint.h>  // 提供uint32_t等标准整数类型定义

void Delay_s(uint32_t s);
void delay_ms(uint32_t ms);
void Delay_us(uint32_t us);


#endif