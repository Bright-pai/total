#ifndef __PID_H_
#define __PID_H_
#include "ti_msp_dl_config.h"

typedef enum
{
	incrementalPid,
	positionPID
}PID_Mode;


typedef struct{

	float now;
	float last;
	float integral;
  float lastlast;

}Error;

typedef struct{

	float kp,ki,kd;
	float target;
	volatile float input,output;
	volatile Error error;
	float MAX_OUTPUT;
	float MAX_ERROR_INTEGRAL;
	float test;
}PID;

void Reset_PID(PID* pid);
float limit(float input,const float MAX_OUTPUT);
void set_PID_Limit(PID* pid,const float MAX_OUTPUT,const float MAX_ERROR_INTEGRAL);
void update_PID(PID* pid,float input,PID_Mode mode,int inte,float limit_num );
void set_PID_Param(PID* pid,float kp,float ki,float kd);
void set_PID_Target(PID* pid,float target);
void init_zero_PID(PID* pid);


#endif /* ENCODER_H_ */
