#ifndef _CHASE_H
#define _CHASE_H
#include "ti_msp_dl_config.h"

#define Switch_Address_0(i) ((i)?(DL_GPIO_setPins(GPIO_TRACE_AD0_PORT,GPIO_TRACE_AD0_PIN )) : (DL_GPIO_clearPins(GPIO_TRACE_AD0_PORT,GPIO_TRACE_AD0_PIN )))// 地址位0控制

#define Switch_Address_1(i) ((i)?(DL_GPIO_setPins(GPIO_TRACE_AD1_PORT,GPIO_TRACE_AD1_PIN )) : (DL_GPIO_clearPins(GPIO_TRACE_AD1_PORT,GPIO_TRACE_AD1_PIN )))// 地址位1控制

#define Switch_Address_2(i) ((i)?(DL_GPIO_setPins(GPIO_TRACE_AD2_PORT,GPIO_TRACE_AD2_PIN )) : (DL_GPIO_clearPins(GPIO_TRACE_AD2_PORT,GPIO_TRACE_AD2_PIN )))// 地址位2控制
typedef struct {
	float error;
	float last_error;
	float kp;
	float ki;
	float kd;
    int32_t output;
  	int tracestate;
	float P;
	float I;
	float D;
} PID_TR_GW;

void CHASE_ADC_Init(void);
bool adc_getvalue(void);
void ganwei_Calibrate(void);
void tr_init_ganwei(PID_TR_GW* pid_tr,float kp,float ki,float  kd);
int32_t pid_gawei_output( PID_TR_GW* pid_tr);
void tr_init_ganwei(PID_TR_GW* pid_tr,float kp,float ki,float  kd);
#endif

