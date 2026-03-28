#ifndef _BUJIN_H
#define _BUJIN_H

#include "ti_msp_dl_config.h"

#define microstepping 6400

typedef struct
{
	double now;
	double last;
}Tim;

/*用于云台与摄像头没有绑在一起*/


typedef struct
{
	Tim stepper_velocity_angle;
	Tim stepper_distance_angle;
	double target_velocity_angle;
	double target_distance_angle;
}Stepper;

void Stepper_Init(Stepper* stepper_x,Stepper* stepper_y);
void Init_Bujin(void);
void Set_Bujin(void);
void Stop_X_Bujin(void);
void Stop_Y_Bujin(void);
void Start_X_Bujin(void);
void Start_Y_Bujin(void);
void Set_X_Bujin_Speed(float Speed);
void Set_Y_Bujin_Speed(float Speed);
void Bujin_Set_X_Num(uint32_t Nuum);//0顺时针
void Bujin_Set_Y_Num(uint32_t Nuum);//0向后
void Pixel_Calibration_To_Angle_Update(Stepper* stepper_x,Stepper* stepper_y,int x,int y,int x0,int y0);
extern uint32_t X_NUM;
extern uint32_t Y_NUM;

extern uint8_t X_complete_Flag;
extern uint8_t Y_complete_Flag;
extern Stepper stepper_x;
extern Stepper stepper_y;

#endif