#ifndef _KEY_H
#define _KEY_H

#include "ti_msp_dl_config.h"
#include "Delay.h"

typedef enum
{
    KEY0,
    KEY1,
    KEY2,
    No_KEY
}Type_KEY;

Type_KEY Key_GetNum(void);
int Key_Detect_Task(void);
int Key_Task1_CircleNum(void);

#endif