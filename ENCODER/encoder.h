#ifndef __ENCODER_H
#define __ENCODER_H
#include "ti_msp_dl_config.h"
#define CLK_TIMER6 32000000
#define PSC_TIMER6 32

//����������
#define reductionRatio 28
#define PI 3.1415926535
#define CMperPAUSE 0.05782
#define ANGLEperPAUSE 0.2472527
#define Timer_IT_Hz  10

//GMR������
#define PAUSEperCIRCLE (14000*4)
#define CMperPAUSE_GMR 2*PI*3.3/PAUSEperCIRCLE
#define ANGLEperPAUSE_GMR 2*PI/PAUSEperCIRCLE

typedef struct{
	float angular;//????????
	float linear;//?????????
}Velocity;


typedef struct{
	double rotations;//???
	double distance;//????
	double angle;//???
}Position;


typedef struct{
	int32_t count_now;//???????
	int32_t count_last;//????��???
	int64_t count_total;//??????
}Count;


typedef struct{
	Velocity velocity;
	Position position;
	Count count;
}Encoder;

void Init_Zero_Encoder(Encoder* encoder_l,Encoder* encoder_r);
void encoder_Init(Encoder* encoder_l,Encoder* encoder_r);
void encoder_update(Encoder* encoder_l,Encoder* encoder_r,const int count_l,const int count_r);
void TIMER_IT_ENABLE(void);
void TIMER_IT_DISENABLE(void);
#endif
