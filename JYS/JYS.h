#ifndef _JYS_H
#define _JYS_H

typedef struct{
float kp;
float ki;
float kd;
volatile float error_now;
volatile float error_last;
volatile float error_lastlast;
volatile float error_integral;
float target;
volatile float output;
}PID_JY901;

void JY61P_Enable(void);
void pid_JY61P_init_Param(PID_JY901* pid_jy901,const float kp,const float ki,const float kd);
void pid_JY61P_init_zero(PID_JY901* pid_jy901);
void Serial_JY61P_Zero_Yaw(void);
void serial_JY61P_zeroXY(void);
void serial_JY61P_AUTO_UNSUAL(void);
void serial_JY61P_SET_HZ(void);
void serial_JY61P_OVERCPME_ZERO(void);



#endif
