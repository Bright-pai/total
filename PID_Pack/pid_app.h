#ifndef	__PID_APP_H
#define __PID_APP_H
#include "ti_msp_dl_config.h"
#include "JYS.h"
#include "pid.h"
#include "encoder.h"
#include "chase.h"
#include "BUJIN.h"
typedef enum
{
	LEFTTYRE=1,
	RIGHTTYRE=2

}MotorType;
typedef enum Position{
	front,behind,left,right
}POS;

#define BEEP_ON()   DL_GPIO_clearPins(GPIO_ADD_PORT, GPIO_ADD_PIN_BEEP_PIN)
#define BEEP_OFF()   DL_GPIO_setPins(GPIO_ADD_PORT, GPIO_ADD_PIN_BEEP_PIN)

#define Laser_ON()  DL_GPIO_setPins(GPIO_ADD_PORT, GPIO_ADD_PIN_LASERPEN_PIN)
#define Laser_OFF()  DL_GPIO_clearPins(GPIO_ADD_PORT, GPIO_ADD_PIN_LASERPEN_PIN)



void IT_PIDapplication_keep_velocity(PID* v_pid,Encoder* encoder,const float v_target,MotorType motortype);
void IT_PIDapplication_keep_velocity_2TYRE(const float v_target);
void IT_PIDapplication_keep_distance(PID* s_pid,PID* v_pid,Encoder* encoder,const float s_target,MotorType motortype,const float v_target);
void IT_PIDapplication_keep_distance_chaliang(PID* s_pid,PID* v_pid,Encoder* encoder,const float d_s_target,MotorType motortype,const float v_target);
void IT_PIDapplication_distance_2tyre_no901(const float  distance,const float v_target);
void go_straight_jy901_keepdistance(PID_JY901* pid_jy901,const float target,const float v_target,const float distance);
void go_straight_jy901_keepvelocity(PID_JY901* pid_jy901,const float target,const float v_target);
void turn_angle_jy901(PID_JY901* pid_jy901,const float target);
void stop(void);
#if TRACE_USE
void trace_go_straight_v(const float v_target,PID_TR* pid_tr);
#endif
void trace_ganwei_go_straight_v(const float v_target,PID_TR_GW* pid_tr);
void turn_angle_encoder(const float angle_line,const float chasu);

void  Stepper_keep_velocity(Stepper* stepper,PID* v_pid,float v_target);
void  Stepper_keep_velocity_reserve(Stepper* stepper,PID* v_pid,float v_target);
void  Stepper_keep_distance(Stepper* stepper,PID* s_pid,float s_target,float v_limit);
void Stepper_keep_distance_Doublecircle(Stepper* stepper,PID* v_pid,PID* s_pid,float s_target,float v_target);
void  Stepper_keep_distance_reserve(Stepper* stepper,PID* s_pid,float s_target,float v_limit);
void Stepper_keep_distance_Doublecircle_reserve(Stepper* stepper,PID* v_pid,PID* s_pid,float s_target,float v_target);

#endif

