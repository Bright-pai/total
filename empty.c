/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include "UART.h"
#include "Motor.h"
#include "Delay.h"
#include "oled.h"
#include "BUJIN.h"
#include "Key.h"
#include "pid.h"
#include "pid_app.h"
#include "chase.h"
#include <string.h>
#include "STP.h"
#include "JYS.h"
#include "SINCOS.h"
#include "encoder.h"
#include "MAXCI.h"



/*电机、编码器全局 */
Encoder encoder_l;
Encoder encoder_r;

uint16_t Left_Dir;
uint16_t Right_Dir;

int EncoderA_Count=0;
int EncoderB_Count=0;

int PWM_L;
int PWM_R;

/*步进电机*/
Stepper stepper_x;
Stepper stepper_y;

float F_Pulse_X;
float F_Pulse_Y;



/*各种PID实例化*/
PID v_pid_l;//�ٶ�PIDʵ����
PID v_pid_r;//�ٶ�PIDʵ����

PID s_pid_l;//λ��PIDʵ����
PID s_pid_r;//λ��PIDʵ����

PID_TR_GW pid_tr;

PID_JY901 pid_jy901;

PID s_X_stepPID;
PID s_Y_stepPID;
PID v_X_stepPID;
PID v_Y_stepPID;

/*敢为数值监测*/
volatile uint16_t oringinal_data[20];
volatile uint16_t anolag_result[8];
volatile uint16_t real_anolog_result[8];
uint16_t white[8];
uint16_t black[8];
uint16_t threshould[8];
uint16_t digital[8];
uint16_t status;
volatile float ganweitr_error;


/*陀螺仪监测值*/
float Yaw;
float Yaw_Last;
float Yaw_Add;

/*视觉*/
#define  Rx_Maxci_Packet_Max 20   
uint8_t Rx_Maxci_Data[Rx_Maxci_Packet_Max];
uint16_t Pixel_X;
uint16_t Pixel_Y;
int Rx_Maxci_TransferNum;



/*任务同步*/
int count_project;
uint32_t MAXCI_tim_cnt_last;
uint32_t MAXCI_tim_cnt_now;
uint32_t MAXCI_tim_transfer_interval;


/*各任务按钮初始化以及标志*/

int TASK_TOTAL;//任务


/*小车状态机以及跳状态函数 */
typedef struct car_param
{
    float distance;
    float chasu;
    float base_velicity;
    int time_cnt_deadline;

}Car_Task_Param;

Car_Task_Param car_param;

typedef enum circle_num 
{
    One,
    Two,
    Three,
    Four,
    Five
}Circle_Num;

typedef enum circle_pos
{
    Front,
    Left,
    Behind,
    Right
}Circle_Pos;

typedef enum movement_state
{
    tr,
    turn
}Movement_State;

Circle_Num Big_State;
Circle_Pos Small_State;
Movement_State movement_state=tr;


/*云台视觉状态*/

#define Standerd_Pos_Num 20
float P_V_X[Standerd_Pos_Num];
float P_V_Y[Standerd_Pos_Num];

float I_V_X[Standerd_Pos_Num];
float I_V_Y[Standerd_Pos_Num];

float D_V_X[Standerd_Pos_Num];
float D_V_Y[Standerd_Pos_Num];


float P_S_X[Standerd_Pos_Num];
float P_S_Y[Standerd_Pos_Num];

float I_S_X[Standerd_Pos_Num];
float I_S_Y[Standerd_Pos_Num];

float D_S_X[Standerd_Pos_Num];
float D_S_Y[Standerd_Pos_Num];


typedef enum yuntai_state
{
    seek,
    PID_Track
}YuntaiState;
YuntaiState yuntai_state;
/***********函数****************/



/*设参数*/
void vSet_carParam(float chasu,float base_velicity,float distance,int time_cnt_deadline)
{
    car_param.base_velicity=base_velicity;
    car_param.chasu=chasu;
    car_param.distance=distance;
    car_param.time_cnt_deadline=time_cnt_deadline;
}


/*跳状态*/
void vJump_CircleState(Circle_Num num,Circle_Pos pos)
{
    Big_State=num;
    Small_State=pos;

}

void vJump_MovementState(Movement_State state)
{
    movement_state=state;
}



/*Task专属*/
/********************************************************TASK1*********************************************************************/
/*TASK1全局变量*/

int turn_task1_cnt=0;
int Set_Flag=0;
int turn_command_num;


/*方案一（转弯）：差速压弯*/
/*差速转弯*/



void chasu_turn_detect_distance(float chasu,float base_velicity,float distance,Circle_Num num,Circle_Pos pos)
{
    if(Set_Flag==1)
    {
        vSet_carParam(chasu,base_velicity,distance,0);
        Set_Flag=0;
    }
    IT_PIDapplication_keep_velocity(&v_pid_l,&encoder_l,car_param.base_velicity+car_param.chasu,LEFTTYRE);
    IT_PIDapplication_keep_velocity(&v_pid_r,&encoder_r,car_param.base_velicity-car_param.chasu,RIGHTTYRE);
    if((encoder_l.position.distance+encoder_r.position.distance)/2>car_param.distance)
    {
        Set_Flag=1;
        vJump_CircleState(num,pos);
        init_zero_PID(&v_pid_l);
        init_zero_PID(&v_pid_r);
        Init_Zero_Encoder(&encoder_l,&encoder_r);
    }
}


/*方案二（转弯）：巡线检测到直角边差速转弯，*/
void ganwei_turn(float base_velicity,float chasu,int time_cnt_deadline)
{
    static int tim_cnt=0;
    if(Set_Flag==1)
    {
        vSet_carParam(chasu,base_velicity,0,time_cnt_deadline);
        Set_Flag=0;
    }
    tim_cnt++;
    IT_PIDapplication_keep_velocity(&v_pid_l,&encoder_l,car_param.base_velicity-car_param.chasu,LEFTTYRE);
    IT_PIDapplication_keep_velocity(&v_pid_r,&encoder_r,car_param.base_velicity+car_param.chasu,RIGHTTYRE);
    if(tim_cnt>time_cnt_deadline)
    {
        tim_cnt=0;
        Set_Flag=1;
        vJump_MovementState(tr);
        init_zero_PID(&v_pid_l);
        init_zero_PID(&v_pid_r);
        Init_Zero_Encoder(&encoder_l,&encoder_r);
        turn_task1_cnt++;
        if(turn_task1_cnt==turn_command_num)
        {
            PWM_L=0;
            PWM_R=0;
            TIMER_IT_DISENABLE();
        }
    }

}

void tr_detect_90_over(float base_velicity,int time_cnt_deadline)
{
    static int tim_cnt1=0;
    if(Set_Flag==1)
    {
        vSet_carParam(0,base_velicity,0,0);
        Set_Flag=0;
    }
    if(((((pid_tr.tracestate)>>7)&1)== 1)&&(((pid_tr.tracestate)&0X03)==0)&&(tim_cnt1>time_cnt_deadline))
    {
        tim_cnt1=0;
        Set_Flag=1;
        vJump_MovementState(turn);
        init_zero_PID(&v_pid_l);
        init_zero_PID(&v_pid_r);
        Init_Zero_Encoder(&encoder_l,&encoder_r);
        
    }
    tim_cnt1++;
    trace_ganwei_go_straight_v(base_velicity,&pid_tr);

}

/************************************************************************TASK2&3***************************************************************************** */
int Pixel_centre_x=224;
/*单环位置，超时启动激光*/
void Task2_IT_SingleCircle_Time(float v_limit,int time_cnt_deadline,int x_error_deadline)
{
    static int time_cnt2=0;
    time_cnt2++;
    Stepper_keep_distance_reserve(&stepper_x,&s_X_stepPID,Pixel_centre_x,v_limit);
    if(time_cnt2>time_cnt_deadline)
    {
        Laser_ON();
    }
}

/*双环位置，超时启动激光*/
void Task2_IT_DoubleCircle_Time(float v_limit,int time_cnt_deadline,int x_error_deadline)
{
    static int time_cnt2=0;
    time_cnt2++;
    Stepper_keep_distance_Doublecircle(&stepper_x,&v_X_stepPID,&s_X_stepPID,Pixel_centre_x,v_limit);
    if(time_cnt2>time_cnt_deadline)
    {
        Laser_ON();
    }
}

/*单环位置，距离到范围启动激光*/
void Task2_IT_SingleCircle_ErrorPos(float v_limit,int time_cnt_deadline,int x_error_deadline)
{
    static int time_cnt2=0;
    time_cnt2++;
    Stepper_keep_distance_reserve(&stepper_x,&s_X_stepPID,Pixel_centre_x,v_limit);
    if(abs_int(Pixel_centre_x-Pixel_X)<x_error_deadline)
    {
        Laser_ON();
    }
}

/*双环位置，距离到范围启动激光*/
void Task2_IT_DoubleCircle_ErrorPos(float v_limit,int time_cnt_deadline,int x_error_deadline)
{
    static int time_cnt2=0;
    time_cnt2++;
    Stepper_keep_distance_Doublecircle(&stepper_x,&v_X_stepPID,&s_X_stepPID,Pixel_centre_x,v_limit);
    if(abs_int(Pixel_centre_x-Pixel_X)<x_error_deadline)
    {
        Laser_ON();
    }
}


/*******************************************************Task3*************************************************** */
Type_KEY start_manner; 
int limit_pos_flag=0;
void Task3_Seek_X(float v_seek)
{
    if(start_manner==KEY1)
        F_Pulse_X=v_seek;
    else if(start_manner==KEY2)
        F_Pulse_X=-v_seek;
     
    if((Pixel_X!=999)&&(Pixel_Y!=999))
    {
        yuntai_state=PID_Track;
    }
}

void Task3_PID_Track_X(float v_limit,int time_cnt_deadline,int x_error_deadline)
{
    static int time_cnt2=0;
    time_cnt2++;
    if((Pixel_X!=999)&&(Pixel_Y!=999))
    {
        MAXCI_tim_cnt_now=DL_Timer_getTimerCount(motor_CLOCK_INST)+10000*count_project;
        MAXCI_tim_transfer_interval=MAXCI_tim_cnt_now-MAXCI_tim_cnt_last;
        MAXCI_tim_cnt_last=MAXCI_tim_cnt_now;
        Pixel_Calibration_To_Angle_Update(&stepper_x,&stepper_y,Pixel_X,Pixel_Y,0,0);
        Stepper_keep_distance_reserve(&stepper_x,&s_X_stepPID,Pixel_centre_x,v_limit);
    }

    if(time_cnt2>time_cnt_deadline)
    {
        Laser_ON();
    }
}

void Task3_Limit_Y(int time_cnt_deadline,float go_speed,float return_speed,int Pulse_Num)
{
    static int time_cnt2=0;
    time_cnt2++;
    if(limit_pos_flag==0)
    {
        F_Pulse_Y=go_speed;
        if(time_cnt2>time_cnt_deadline)
        {
            limit_pos_flag=1;
            NVIC_ClearPendingIRQ(PWM_BUJIN_Y_INST_INT_IRQN);
            NVIC_EnableIRQ(PWM_BUJIN_Y_INST_INT_IRQN);
            Bujin_Set_Y_Num(Pulse_Num);
            F_Pulse_Y=0;
        }
    }
    if((limit_pos_flag==1)&&(time_cnt2>(time_cnt_deadline+10)))
    {
        F_Pulse_Y=-return_speed;
        if(Y_NUM==0)
        {
            F_Pulse_Y=0;
            limit_pos_flag=2;
            NVIC_DisableIRQ(PWM_BUJIN_Y_INST_INT_IRQN);
        }
    }


}


/****************************************************************Task4********************************************************* */
float kai_huan1[5]={700,3000,6400,6400,6400};
float kai_huan2[5]={1,1,1,1,1};
float kai_huan3[5]={1,1,1,1,1};
float kai_huan4[5]={1,1,1,1,1};

float tim1[6]={};
float tim2[6]={};
float tim3[6]={};
float tim4[6]={};

int tr_project_tim_cnt;
int turn_project_tim_cnt;
float dynamics_PID_Param[8][3]={
    {100,0,0},/*1*//*turn*/
    {3,0.3,0.3},/*2*//*tr*/
    {2,0,0},/*3*//*turn*/
    {11,0,0},/*4*//*tr*/
    {8,0,0},/*5*//*turn*/
    {2,0,0},/*6*//*tr*/
    {16,0,0},/*7*//*turn*/
    {22,0,0} /*8*//*tr*/
};
float start_turnPID[3]={50,0,0};
float small_turnPID[3]={50,0,0};
float big_turnPID[3]={50,0,0};
int tr_task4_cnt=0;
float Yaw_Target=90.0f;
void Task4_ganwei_turn(float base_velicity,float chasu,int time_cnt_deadline)
{
    static int tim_cnt=0;
    if(Set_Flag==1)
    {

        int i=tr_task4_cnt%4;
        set_PID_Param(&s_X_stepPID,dynamics_PID_Param[i*2][0],dynamics_PID_Param[i*2][1],dynamics_PID_Param[i*2][2]);
        vSet_carParam(chasu,base_velicity,0,time_cnt_deadline);
        Set_Flag=0;
    }
    tim_cnt++;
    IT_PIDapplication_keep_velocity(&v_pid_l,&encoder_l,car_param.base_velicity-car_param.chasu,LEFTTYRE);
    IT_PIDapplication_keep_velocity(&v_pid_r,&encoder_r,car_param.base_velicity+car_param.chasu,RIGHTTYRE);
    if(tim_cnt>time_cnt_deadline)
    {
        tim_cnt=0;
        Set_Flag=1;
        vJump_MovementState(tr);
        init_zero_PID(&v_pid_l);
        init_zero_PID(&v_pid_r);
        init_zero_PID(&s_X_stepPID);
        Init_Zero_Encoder(&encoder_l,&encoder_r);

    
    }

}

void Task4_tr_detect_90_over(float base_velicity,int  time_cnt_deadline)
{
    static int tim_cnt1=0;

    if(Set_Flag==1)
    {

        int i=tr_task4_cnt%4;
        set_PID_Param(&s_X_stepPID,dynamics_PID_Param[i*2+1][0],dynamics_PID_Param[i*2+1][1],dynamics_PID_Param[i*2+1][2]);
        vSet_carParam(0,base_velicity,0,0);
        Set_Flag=0;
    }
    tim_cnt1++;
    trace_ganwei_go_straight_v(base_velicity,&pid_tr);
    if(((((pid_tr.tracestate)>>7)&1)== 1)&&(((pid_tr.tracestate)&0X03)==0)&&(tim_cnt1>time_cnt_deadline))
    {
        if((Yaw+90)>180)
            Yaw_Target=Yaw+90-360;
        else
            Yaw_Target=Yaw+90;
        tr_task4_cnt++;
        Set_Flag=1;
        if(tr_task4_cnt==turn_command_num)
        {
            PWM_L=0;
            PWM_R=0;
        }
        else 
        {
            vJump_MovementState(turn);
            init_zero_PID(&v_pid_l);
            init_zero_PID(&v_pid_r);
            Init_Zero_Encoder(&encoder_l,&encoder_r);
        }
        
    }
}

void Task4_turn_first(int l_v_limit,int r_v_limit,int l_add,int r_add,int  time_cnt_deadline)
{
    static int tim_cnt=0;
    if(Set_Flag==1)
    {
        Set_Flag=0;
    }
    tim_cnt++;
    PWM_L=PWM_L+l_add;
    PWM_R=PWM_R+r_add;
    if(PWM_L>l_v_limit)
        PWM_L=l_v_limit;
    if(PWM_R>r_v_limit)
        PWM_R=r_v_limit;
    if(tim_cnt>time_cnt_deadline)
    {
        tim_cnt=0;
        Set_Flag=1;
        vJump_MovementState(tr);
        init_zero_PID(&v_pid_l);
        init_zero_PID(&v_pid_r);
        init_zero_PID(&s_X_stepPID);
        Init_Zero_Encoder(&encoder_l,&encoder_r);

    
    }

}

void Task4_IT_SingleCircle_ErrorPos(float v_limit)
{
    static int change_flag=0;

    if(movement_state==turn)
    {
        // if(((tr_task4_cnt%4)==0)&&(((count_project-project_cnt_sym)%180)==0))
        // {
        //     F_Pulse_X=kai_huan1[(count_project-project_cnt_sym)/180];
        // }
        // else if(((tr_task4_cnt%4)==1)&&(((count_project-project_cnt_sym)%160)==0))
        // {
        //     F_Pulse_X=kai_huan2[(count_project-project_cnt_sym)/160];
        // }
        // else if(((tr_task4_cnt%4)==2)&&(((count_project-project_cnt_sym)%160)==0))
        // {
        //     F_Pulse_X=kai_huan3[(count_project-project_cnt_sym)/160];
        // }
        // else if(((tr_task4_cnt%4)==3)&&(((count_project-project_cnt_sym)%160)==0))
        // {
        //     F_Pulse_X=kai_huan4[(count_project-project_cnt_sym)/160];
        // }

    }
    else
        Stepper_keep_distance_reserve(&stepper_x,&s_X_stepPID,Pixel_centre_x,v_limit);
    if(tr_task4_cnt==turn_command_num)
    {
        F_Pulse_X=0;
        F_Pulse_Y=0;
        
        TIMER_IT_DISENABLE();
    }

}


void TASK1_MAIN(void)
{
    turn_command_num=Key_Task1_CircleNum();
    turn_command_num=turn_command_num*4;

    Motor_Init();
    encoder_Init(&encoder_l,&encoder_r);
    set_PID_Limit(&v_pid_l,15000,15000);//PWM����޷�
    set_PID_Limit(&v_pid_r,15000,15000);//
    set_PID_Param(&v_pid_l,30.5,2.8,0.3);
    set_PID_Param(&v_pid_r,30.5,2.8,0.3);

	
	set_PID_Limit(&s_pid_l,240,5000);
	set_PID_Limit(&s_pid_r,240,5000);
	set_PID_Param(&s_pid_l,1.5,0.01,0);
	set_PID_Param(&s_pid_r,1.5,0.01,0);



    set_PID_Param(&v_X_stepPID,2,0,0);
	set_PID_Limit(&v_X_stepPID,100000,10000);
	set_PID_Param(&s_X_stepPID,2,0,0);
	set_PID_Limit(&s_X_stepPID,100,4000);
	
	set_PID_Param(&v_Y_stepPID,2,0,0);
	set_PID_Limit(&v_Y_stepPID,100000,10000);
	set_PID_Param(&s_Y_stepPID,2,0,0);
	set_PID_Limit(&s_Y_stepPID,100,4000);

    tr_init_ganwei(&pid_tr,0.31,0,0.10);

    CHASE_ADC_Init();
    ganwei_Calibrate();

    while(!(Key_GetNum()==KEY2));
    OLED_ShowString(30,4,(uint8_t*)"Start",16);

    TIMER_IT_ENABLE();

    movement_state=tr;

    uint8_t lko = 5;

    while (1) 
    {  	

         OLED_ShowString(0,6,(uint8_t*)"State",16);
         OLED_ShowNum(50,6,(int)movement_state,3,16);
         OLED_ShowNum(80,6,(int)turn_task1_cnt,3,16);
         OLED_ShowNum(80,1,(int)pid_tr.tracestate,3,16);

	}

}



void TASK2_MAIN(void)
{
    Motor_Init();
    encoder_Init(&encoder_l,&encoder_r);

/*车PID*/
    set_PID_Limit(&v_pid_l,100000,10000);//PWM����޷�
    set_PID_Limit(&v_pid_r,100000,10000);//
    set_PID_Param(&v_pid_l,6.5,0.006,0);
    set_PID_Param(&v_pid_r,6.5,0.006,0);

	
	set_PID_Limit(&s_pid_l,240,5000);
	set_PID_Limit(&s_pid_r,240,5000);
	set_PID_Param(&s_pid_l,1.5,0.01,0);
	set_PID_Param(&s_pid_r,1.5,0.01,0);


/*云台PID*/
    set_PID_Param(&v_X_stepPID,2,0,0);
	set_PID_Limit(&v_X_stepPID,100000,10000);
	set_PID_Param(&s_X_stepPID,8,0,6);
	set_PID_Limit(&s_X_stepPID,2000000,4000);
	
	set_PID_Param(&v_Y_stepPID,2,0,0);
	set_PID_Limit(&v_Y_stepPID,100000,10000);
	set_PID_Param(&s_Y_stepPID,2,0,0);
	set_PID_Limit(&s_Y_stepPID,5000,4000);

    tr_init_ganwei(&pid_tr,4,0,0);

    MAXIC_RX_Num_Set(9);
    MAXIC_Enable();
    while(!(Key_GetNum()==KEY2));
    OLED_ShowString(30,2,(uint8_t*)"Start",16);
    Init_Bujin();
    Set_Bujin();


    JY61P_Enable();
    TIMER_IT_ENABLE();
    movement_state=tr;

    //Draw_Circle(0, 0, 5, 0, 3000, 60);
    while (1) 
    {  	

        OLED_ShowNum(80,1,(int)pid_tr.tracestate,3,16);
        /*task2显示*/
        if((Pixel_X-Pixel_centre_x)<0)
        {
            OLED_ShowString(0,6,(uint8_t*)"x_error",16);
            OLED_ShowString(70,6,(uint8_t*)"-",16);
            OLED_ShowNum(80,6,(int)(Pixel_centre_x-Pixel_X),3,16);
        }
        else 
        {
            OLED_ShowString(0,6,(uint8_t*)"x_error",16);
            OLED_ShowString(70,6,(uint8_t*)"+",16);
            OLED_ShowNum(80,6,(int)(Pixel_X-Pixel_centre_x),3,16);
        }
        if(F_Pulse_X<0)
        {
            OLED_ShowString(0,4,(uint8_t*)"-",16);
            OLED_ShowNum(20,4,(int)(-F_Pulse_X),5,16);
        }
        else 
        {
            OLED_ShowString(0,4,(uint8_t*)"+",16);
            OLED_ShowNum(20,4,(int)(F_Pulse_X),5,16);
        }
        
        //OLED_ShowNum(0,4,(uint32_t)(MAXCI_tim_transfer_interval),10,16);

	}
}

void TASK3_MAIN(void)
{
    Motor_Init();
    encoder_Init(&encoder_l,&encoder_r);

/*车PID*/
    set_PID_Limit(&v_pid_l,100000,10000);//PWM����޷�
    set_PID_Limit(&v_pid_r,100000,10000);//
    set_PID_Param(&v_pid_l,6.5,0.006,0);
    set_PID_Param(&v_pid_r,6.5,0.006,0);

	
	set_PID_Limit(&s_pid_l,240,5000);
	set_PID_Limit(&s_pid_r,240,5000);
	set_PID_Param(&s_pid_l,1.5,0.01,0);
	set_PID_Param(&s_pid_r,1.5,0.01,0);


/*云台PID*/
    set_PID_Param(&v_X_stepPID,2,0,0);
	set_PID_Limit(&v_X_stepPID,100000,10000);
	set_PID_Param(&s_X_stepPID,10,0,12);
	set_PID_Limit(&s_X_stepPID,5000,4000);
	
	set_PID_Param(&v_Y_stepPID,2,0,0);
	set_PID_Limit(&v_Y_stepPID,100000,10000);
	set_PID_Param(&s_Y_stepPID,2,0,0);
	set_PID_Limit(&s_Y_stepPID,5000,4000);

    tr_init_ganwei(&pid_tr,4,0,0);

    MAXIC_RX_Num_Set(9);
    MAXIC_Enable();
    Init_Bujin();
    Set_Bujin();
    start_manner=Key_GetNum();
    while(!((start_manner==KEY1)||(start_manner==KEY2)))
    {
        start_manner=Key_GetNum();
    }
    OLED_ShowString(30,2,(uint8_t*)"Start",16);
    TIMER_IT_ENABLE();
    movement_state=tr;
    yuntai_state=seek;
    while (1) 
    {  	
        OLED_ShowNum(80,1,(int)pid_tr.tracestate,3,16);
        /*task2显示*/
        if((Pixel_X-Pixel_centre_x)<0)
        {
            OLED_ShowString(0,6,(uint8_t*)"x_error",16);
            OLED_ShowString(70,6,(uint8_t*)"-",16);
            OLED_ShowNum(80,6,(int)(Pixel_centre_x-Pixel_X),3,16);
        }
        else 
        {
            OLED_ShowString(0,6,(uint8_t*)"x_error",16);
            OLED_ShowString(70,6,(uint8_t*)"+",16);
            OLED_ShowNum(80,6,(int)(Pixel_X-Pixel_centre_x),3,16);
        }
        if(F_Pulse_X<0)
        {
            OLED_ShowString(0,4,(uint8_t*)"-",16);
            OLED_ShowNum(20,4,(int)(-F_Pulse_X),5,16);
        }
        else 
        {
            OLED_ShowString(0,4,(uint8_t*)"+",16);
            OLED_ShowNum(20,4,(int)(F_Pulse_X),5,16);
        }
        
        //OLED_ShowNum(0,4,(uint32_t)(MAXCI_tim_transfer_interval),10,16);

	}
}

void TASK4_MAIN(void)
{
    turn_command_num=4;
    Motor_Init();
    encoder_Init(&encoder_l,&encoder_r);

/*车PID*/
    set_PID_Limit(&v_pid_l,100000,10000);//PWM����޷�
    set_PID_Limit(&v_pid_r,100000,10000);//
    set_PID_Param(&v_pid_l,30.5,2.9,0.3);
    set_PID_Param(&v_pid_r,30.5,2.9,0.3);

	
	set_PID_Limit(&s_pid_l,240,5000);
	set_PID_Limit(&s_pid_r,240,5000);
	set_PID_Param(&s_pid_l,1.5,0.01,0);
	set_PID_Param(&s_pid_r,1.5,0.01,0);


/*云台PID*/
    set_PID_Param(&v_X_stepPID,2,0,0);
	set_PID_Limit(&v_X_stepPID,100000,10000);
	set_PID_Param(&s_X_stepPID,0,0,0);
	set_PID_Limit(&s_X_stepPID,5000,4000);
	
	set_PID_Param(&v_Y_stepPID,2,0,0);
	set_PID_Limit(&v_Y_stepPID,100000,10000);
	set_PID_Param(&s_Y_stepPID,2,0,0);
	set_PID_Limit(&s_Y_stepPID,5000,4000);

    tr_init_ganwei(&pid_tr,0.31,0,0);

    pid_JY61P_init_Param(&pid_jy901,60,0,0);


    JY61P_Enable();
    CHASE_ADC_Init();
    ganwei_Calibrate();

    MAXIC_RX_Num_Set(9);
    MAXIC_Enable();

    while(!(Key_GetNum()==KEY2));
    OLED_ShowString(21,2,(uint8_t*)"S",16);
    Init_Bujin();
    Set_Bujin();
    Laser_ON();

    while(!(Key_GetNum()==KEY2));
    OLED_ShowString(30,2,(uint8_t*)"Start",16);
    Serial_JY61P_Zero_Yaw();
    Set_Flag=1;
    TIMER_IT_ENABLE();
    movement_state=turn;


    while (1) 
    {  	
        OLED_ShowNum(80,1,(int)pid_tr.tracestate,3,16);
        /*task2显示*/
        OLED_ShowString(0,6,(uint8_t*)"x_error",16);
        if((Pixel_X-Pixel_centre_x)<0)
        {
            OLED_ShowString(70,6,(uint8_t*)"-",16);
            OLED_ShowNum(80,6,(int)(Pixel_centre_x-Pixel_X),3,16);
        }
        else 
        {
            OLED_ShowString(70,6,(uint8_t*)"+",16);
            OLED_ShowNum(80,6,(int)(Pixel_X-Pixel_centre_x),3,16);
        }

        OLED_ShowString(0,4,(uint8_t*)"V",16);
        if(F_Pulse_X<0)
        {
            OLED_ShowString(10,4,(uint8_t*)"-",16);
            OLED_ShowNum(20,4,(int)(-F_Pulse_X),4,16);
        }
        else 
        {
            OLED_ShowString(10,4,(uint8_t*)"+",16);
            OLED_ShowNum(20,4,(int)(F_Pulse_X),4,16);
        }

        OLED_ShowNum(60,4,(int)(pid_tr.tracestate),3,16);

        
        if(Yaw<0)
        {
            OLED_ShowString(90,4,(uint8_t*)"-",16);
            OLED_ShowNum(100,4,(int)(-Yaw),3,16);
        }
        else 
        {
            OLED_ShowString(90,4,(uint8_t*)"+",16);
            OLED_ShowNum(100,4,(int)(Yaw),3,16);
        }
        
        
        //OLED_ShowNum(0,4,(uint32_t)(MAXCI_tim_transfer_interval),10,16);

	}

}





/************************************************************IT************************************************** */
void TASK1_IT(void)
{
        count_project++;
        encoder_update(&encoder_l, &encoder_r, EncoderA_Count, EncoderB_Count);
           //  IT_PIDapplication_keep_velocity(&v_pid_l,&encoder_l,3,LEFTTYRE);
	       //  IT_PIDapplication_keep_velocity(&v_pid_r,&encoder_r,3,RIGHTTYRE);
        switch(movement_state)
        {
            case tr:
            {
                tr_detect_90_over(2.5,50);
                break;
            }
            case turn:
            {
                ganwei_turn(1.4,1.0,80);
                break;
            }

        }
            //pid_gawei_output(&pid_tr);
        //trace_ganwei_go_straight_v(3,&pid_tr);
        EncoderA_Count = 0;
        EncoderB_Count = 0;

        Set_Left_Motor(PWM_L);
        Set_Right_Motor(PWM_R);
}

void TASK2_IT(void)
{
    count_project++;
    encoder_update(&encoder_l, &encoder_r, EncoderA_Count, EncoderB_Count);

    if((Pixel_X!=999)&&(Pixel_Y!=999))
    {
        MAXCI_tim_cnt_now=DL_Timer_getTimerCount(motor_CLOCK_INST)+10000*count_project;
        MAXCI_tim_transfer_interval=MAXCI_tim_cnt_now-MAXCI_tim_cnt_last;
        MAXCI_tim_cnt_last=MAXCI_tim_cnt_now;
        Pixel_Calibration_To_Angle_Update(&stepper_x,&stepper_y,Pixel_X,Pixel_Y,0,0);
        Task2_IT_SingleCircle_Time(2000000,150,0);
        //Stepper_keep_distance_reserve(&stepper_x,&s_X_stepPID,Pixel_centre_x,2000000);
    }
    Set_X_Bujin_Speed(F_Pulse_X);
    Set_Y_Bujin_Speed(F_Pulse_Y);

    EncoderA_Count = 0;
    EncoderB_Count = 0;

}

void TASK3_IT(void)
{
    count_project++;
    encoder_update(&encoder_l, &encoder_r, EncoderA_Count, EncoderB_Count);

    EncoderA_Count = 0;
    EncoderB_Count = 0;

    switch(yuntai_state)
    {
        case seek:
        {
            Task3_Seek_X(3500);
        }
        break;
        case PID_Track:
        {
            Task3_PID_Track_X(6000,350,1);
        }
        break;
    }
    Task3_Limit_Y(160,350,250,380);
    Set_X_Bujin_Speed(F_Pulse_X);
    Set_Y_Bujin_Speed(F_Pulse_Y);
}

void TASK4_IT(void)
{
    count_project++;
    if(count_project>2)
    {
        encoder_update(&encoder_l, &encoder_r, EncoderA_Count, EncoderB_Count);
        switch(movement_state)
        {
            case tr:
            {
                tr_project_tim_cnt++;
                turn_project_tim_cnt=0;
                //DL_GPIO_clearPins(GPIO_STBY_PORT, GPIO_STBY_PIN_1_PIN);
                Task4_tr_detect_90_over(2.325,50);
            }
            break;
            case turn:
            {
                tr_project_tim_cnt=0;
                turn_project_tim_cnt++;
                if(tr_task4_cnt==0)
                    Task4_turn_first(7,24,4,12,95);
                else
                {
                    // DL_GPIO_clearPins(GPIO_STBY_PORT, GPIO_STBY_PIN_1_PIN);
                
                    Task4_ganwei_turn(1.4,1.0,81);
                }
            }
            break;

        }
        //trace_ganwei_go_straight_v(3,&pid_tr);
        EncoderA_Count = 0;
        EncoderB_Count = 0;
    }
    if((Pixel_X!=999)&&(Pixel_Y!=999))
    {
        Pixel_Calibration_To_Angle_Update(&stepper_x,&stepper_y,Pixel_X,Pixel_Y,0,0);
        Task4_IT_SingleCircle_ErrorPos(3200);
        //F_Pulse_Y=1000;
        //Task2_IT_SingleCircle_Time(50,200,0);

    }
    Set_Left_Motor(PWM_L);
    Set_Right_Motor(PWM_R);
    Set_X_Bujin_Speed(F_Pulse_X);
    if(tr_task4_cnt==turn_command_num)
    {

        TIMER_IT_DISENABLE();
    }
    Set_Y_Bujin_Speed(F_Pulse_Y);

}


/**/


int main(void)
{   
    SYSCFG_DL_init();
    OLED_Init();
	OLED_Clear();
    TASK_TOTAL=Key_Detect_Task();
    switch(TASK_TOTAL)
    {
        case 1:
        {
            TASK1_MAIN();
        }
        break;
        case 2:
        {
            TASK2_MAIN();
        }
        break;
        case 3:
        {
            TASK3_MAIN();
        }
        break;
        case 4:
        {
            TASK4_MAIN();
        }
        break;

    }

}







void motor_CLOCK_INST_IRQHandler(void)
{
	switch (DL_TimerG_getPendingInterrupt(motor_CLOCK_INST)) {
		case DL_TIMER_IIDX_ZERO:
        {
            switch(TASK_TOTAL)
            {
                case 1:
                {
                    TASK1_IT();
                }
                break;
                case 2:
                {
                    TASK2_IT();
                }
                break;
                case 3:
                {
                    TASK3_IT();
                }
                break;
                case 4:
                {
                    TASK4_IT();
                }
                break;
            }   
        }
			break;
		default:
			break;	
	}




}




void UART_0_INST_IRQHandler(void)
{   uint8_t ERRO;
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) 
    {
        case DL_UART_IIDX_DMA_DONE_RX:
        {
                static int flag=0;
                DL_DMA_disableChannel(DMA, DMA_MAXCI_CHAN_ID);
                //Pixel_X=Rx_Maxci_Data[0];
                if (!DL_UART_isRXFIFOEmpty(UART_0_INST)) 
                {
                    ERRO = DL_UART_receiveData(UART_0_INST);
                    DL_DMA_setSrcAddr(DMA, DMA_MAXCI_CHAN_ID, (uint32_t)(&UART_0_INST->RXDATA));
                    DL_DMA_setDestAddr(DMA, DMA_MAXCI_CHAN_ID, (uint32_t)&Rx_Maxci_Data[0]);
                    DL_DMA_setTransferSize(DMA,DMA_MAXCI_CHAN_ID,Rx_Maxci_TransferNum);
                    DL_DMA_enableChannel(DMA, DMA_MAXCI_CHAN_ID);
                    
                }
                else if ((Rx_Maxci_Data[0] == 97)&& (Rx_Maxci_Data[1] ==100)) 
                {
                    MAXCI_tim_cnt_now=DL_Timer_getTimerCount(motor_CLOCK_INST)+10000*count_project;
                    MAXCI_tim_transfer_interval=MAXCI_tim_cnt_now-MAXCI_tim_cnt_last;
                    MAXCI_tim_cnt_last=MAXCI_tim_cnt_now;
                    Pixel_X=(Rx_Maxci_Data[2]-48)*100+(Rx_Maxci_Data[3]-48)*10+Rx_Maxci_Data[4]-48;
                    Pixel_Y=(Rx_Maxci_Data[5]-48)*100+(Rx_Maxci_Data[6]-48)*10+Rx_Maxci_Data[7]-48;
                //    //Serial_Printf("%d,%d\n",Pixel_X,Pixel_Y);
                //     if((Pixel_X!=999)&&(Pixel_Y!=999))
                //     {
                //         MAXCI_tim_cnt_now=DL_Timer_getTimerCount(motor_CLOCK_INST)+10000*count_project;
                //         MAXCI_tim_transfer_interval=MAXCI_tim_cnt_now-MAXCI_tim_cnt_last;
                //         MAXCI_tim_cnt_last=MAXCI_tim_cnt_now;
                //         Pixel_Calibration_To_Angle_Update(&stepper_x,&stepper_y,Pixel_X,Pixel_Y,0,0);
                //         Stepper_keep_distance_reserve(&stepper_x,&s_X_stepPID,Pixel_centre_x,6000);
                //         //F_Pulse_Y=1000;
                //         //Task2_IT_SingleCircle_Time(50,200,0);

                //     }
                //     Set_X_Bujin_Speed(F_Pulse_X);
                //     Set_Y_Bujin_Speed(F_Pulse_Y);
                    DL_DMA_setSrcAddr(DMA, DMA_MAXCI_CHAN_ID, (uint32_t)(&UART_0_INST->RXDATA));
                    DL_DMA_setDestAddr(DMA, DMA_MAXCI_CHAN_ID, (uint32_t)&Rx_Maxci_Data[0]);
                    DL_DMA_setTransferSize(DMA,DMA_MAXCI_CHAN_ID,Rx_Maxci_TransferNum);
                    DL_DMA_enableChannel(DMA, DMA_MAXCI_CHAN_ID);
                }
                else if ((Rx_Maxci_Data[1]==98 ) &&(flag==1))
                {
                    flag=0;
                    DL_DMA_setSrcAddr(DMA, DMA_MAXCI_CHAN_ID, (uint32_t)(&UART_0_INST->RXDATA));
                    DL_DMA_setDestAddr(DMA, DMA_MAXCI_CHAN_ID, (uint32_t)&Rx_Maxci_Data[0]);
                    DL_DMA_setTransferSize(DMA,DMA_MAXCI_CHAN_ID,Rx_Maxci_TransferNum);
                    DL_DMA_enableChannel(DMA, DMA_MAXCI_CHAN_ID);
                }
                else 
                {
                    flag=1;
                    //SYSCFG_DL_DMA_init();
                    DL_DMA_setSrcAddr(DMA, DMA_MAXCI_CHAN_ID, (uint32_t)(&UART_0_INST->RXDATA));
                    DL_DMA_setDestAddr(DMA, DMA_MAXCI_CHAN_ID, (uint32_t)&Rx_Maxci_Data[0]);
                    DL_DMA_setTransferSize(DMA,DMA_MAXCI_CHAN_ID,2);
                    DL_DMA_enableChannel(DMA, DMA_MAXCI_CHAN_ID);
                }
        }
        break;
        case DL_UART_MAIN_IIDX_RX_TIMEOUT_ERROR:
        {
            DL_DMA_disableChannel(DMA,DMA_MAXCI_CHAN_ID);//关掉dma，防止dma继续搬运
            while(!DL_UART_isRXFIFOEmpty(UART_0_INST))
            {
                ERRO = DL_UART_receiveData(UART_0_INST);
                //DL UART rainRXFIFO(UART 3 INsT,err buff,3);//读出fifo剩余的数据放在err buffu3 ch =DL UART receiveData(UART 3 INST);
            }
            DL_DMA_setSrcAddr(DMA, DMA_MAXCI_CHAN_ID, (uint32_t)(&UART_0_INST->RXDATA));
            DL_DMA_setDestAddr(DMA, DMA_MAXCI_CHAN_ID, (uint32_t)&Rx_Maxci_Data[0]);
            DL_DMA_setTransferSize(DMA,DMA_MAXCI_CHAN_ID,Rx_Maxci_TransferNum);
            DL_DMA_enableChannel(DMA, DMA_MAXCI_CHAN_ID);

        }
        default:
            break; 
                  
    }
}

void Encoder1A_INST_IRQHandler(void)
{
	
    switch (DL_TimerG_getPendingInterrupt(Encoder1A_INST)) {
        case DL_TIMER_IIDX_CC1_DN:
       
            if(DL_GPIO_readPins(GPIO_Encoder_Encoder1B_PORT,GPIO_Encoder_Encoder1B_PIN))  {
                Left_Dir=0;			
                EncoderA_Count++;
            }
            else {
                Left_Dir=1;
                EncoderA_Count--;
            }
			break;
		
        default:
            break;
    }
}

void Encoder2A_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(Encoder2A_INST)) {
        case DL_TIMER_IIDX_CC0_DN:
            if (DL_GPIO_readPins(GPIO_Encoder_Encoder2B_PORT,GPIO_Encoder_Encoder2B_PIN)) {
                Right_Dir=0;
                EncoderB_Count--;
            }
            else {
                Right_Dir=1;    
                EncoderB_Count++;
            }      
			break; 
		
        default:
            break;     
                    
    }
}



