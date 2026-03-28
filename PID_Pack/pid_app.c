#include "encoder.h"
#include "Motor.h"
#include "pid.h"
#include "pid_app.h"
#include "chase.h"

//#include "hwt101.h"

extern Encoder encoder_l;//������������ʵ������
extern Encoder encoder_r;//������������ʵ������

extern PID v_pid_l;//�ٶ�PIDʵ����
extern PID v_pid_r;//�ٶ�PIDʵ����

extern PID s_pid_l;//λ��PIDʵ����
extern PID s_pid_r;//λ��PIDʵ����

extern PID_TR_GW pid_tr;

extern int PWM_L;
extern int PWM_R;

//extern PID_TR pidtr;
//extern float chasu;
extern uint32_t count_project;
extern PID_JY901 pid_jy901;
extern float Yaw;

#if TRACE_USE
extern PID_TR pid_tr;
#endif
/**************************************************直流电机*********************************************************/

/*
速度单轮增量
参数:速度PID，左右轮编码器，左右轮目标速度
调法：
	kp:kp越大，稳态误差（目标-平稳实际）越小，超调越大，但稳态误差不可能为零
	ki:ki越大，稳态误差消除越快，但系统滞后性越大
	kd:kd越大，越可以消除波动
*/
void IT_PIDapplication_keep_velocity(PID* v_pid,Encoder* encoder,const float v_target,MotorType motortype)
{
	set_PID_Target(v_pid,v_target);
	update_PID(v_pid,encoder->velocity.linear,positionPID ,0,0);//λ��PID
	if(motortype==LEFTTYRE)
		PWM_L=v_pid->output;
	else if(motortype==RIGHTTYRE)
		PWM_R=v_pid->output;		
}	

/*2轮速度*/
void IT_PIDapplication_keep_velocity_2TYRE(const float v_target)
{
	IT_PIDapplication_keep_velocity(&v_pid_l,&encoder_l,v_target,LEFTTYRE);
	IT_PIDapplication_keep_velocity(&v_pid_r,&encoder_r,v_target,RIGHTTYRE);

}

/*
位置双环(单轮)
参数：轮位置PID，速度PID，编码器，目标位置,最高速度（绝对值）
*/
void IT_PIDapplication_keep_distance(PID* s_pid,PID* v_pid,Encoder* encoder,const float s_target,MotorType motortype,const float v_target)
{
	//v_pid->MAX_OUTPUT=v_target;
	set_PID_Target(s_pid,s_target);
	update_PID(s_pid,encoder->position.distance,positionPID,0,0);
	s_pid->output=limit(s_pid->output,v_target);
  if((s_pid->output<=0.5f)&&(s_pid->output>=-0.5f))
        s_pid->output=0;
	if(count_project%2==0)
		IT_PIDapplication_keep_velocity(v_pid,encoder ,s_pid->output,motortype);
}

/*位置双环，输入位置差量*/
void IT_PIDapplication_keep_distance_chaliang(PID* s_pid,PID* v_pid,Encoder* encoder,const float d_s_target,MotorType motortype,const float v_target)
{
	v_pid->MAX_OUTPUT=v_target;
	set_PID_Target(s_pid,d_s_target);
	update_PID(s_pid,0,positionPID,0,0);
  if((s_pid->output<=0.5f)&&(s_pid->output>=-0.5f))
        s_pid->output=0;
	if(count_project%2==0)
		IT_PIDapplication_keep_velocity(v_pid,encoder ,s_pid->output,motortype);
}


/*位置双环，2轮无陀螺仪,距离速度输入绝对值*/
void IT_PIDapplication_distance_2tyre_no901(const float  distance,const float v_target)
{

		IT_PIDapplication_keep_distance(&s_pid_l,&v_pid_l,&encoder_l,distance,LEFTTYRE,v_target);
		IT_PIDapplication_keep_distance(&s_pid_r,&v_pid_r,&encoder_r,distance,RIGHTTYRE,v_target);

}

#if TRACE_USE
/*
�ٶȻ�Ѳ��
*/

void trace_go_straight_v(const float v_target,PID_TR* pid_tr)
{   
    

    const int chasu=pid_calc_output();
    if(pid_tr->tracestate==0)
    {
        float balance;
        balance=(encoder_l.velocity.linear+encoder_r.velocity.linear)/2;
        encoder_l.velocity.linear=balance;
        encoder_r.velocity.linear=balance;
    }
    
    IT_PIDapplication_keep_velocity(&v_pid_l,&encoder_l,v_target,LEFTTYRE);
		IT_PIDapplication_keep_velocity(&v_pid_r,&encoder_r,v_target,RIGHTTYRE);
    if(encoder_l.velocity.linear>10)
    {
        PWM_L=(int)(v_pid_l.output+chasu);
        PWM_R=(int)(v_pid_r.output-chasu);
    }
    else
    {
    	PWM_L=(int)v_pid_l.output;
			PWM_R=(int)v_pid_r.output;
    }
}


#endif

/*敢为巡线*/
void trace_ganwei_go_straight_v(const float v_target,PID_TR_GW* pid_tr)
{   
    const int chasu=pid_gawei_output(pid_tr);
    if(pid_tr->tracestate==0)
    {
        float balance;
        balance=(encoder_l.velocity.linear+encoder_r.velocity.linear)/2;
        encoder_l.velocity.linear=balance;
        encoder_r.velocity.linear=balance;
    }
    
    IT_PIDapplication_keep_velocity(&v_pid_l,&encoder_l,v_target+chasu,LEFTTYRE);
		IT_PIDapplication_keep_velocity(&v_pid_r,&encoder_r,v_target-chasu,RIGHTTYRE);
    //if(encoder_l.velocity.linear>10)
    //{
        // PWM_L=(int)(v_pid_l.output+chasu);
        // PWM_R=(int)(v_pid_r.output-chasu);
    //}
    //else
    //{
    	//PWM_L=(int)v_pid_l.output;
			//PWM_R=(int)v_pid_r.output;
    //}
}

/*
陀螺仪差速pid走直线固定速度固定距离串级速度环（此参数尽量小，辅助作用）
*/
void go_straight_jy901_keepdistance(PID_JY901* pid_jy901,const float target,const float v_target,const float distance)
{
    int manner,error;   
    pid_jy901->target=target;
    pid_jy901->error_last=pid_jy901->error_now;
    error=target-Yaw;
    manner=abs_int(error);
    if(manner>=180)
    {
        if(error>=0)
            pid_jy901->error_now=-(360-manner);
        else if(error<0)
            pid_jy901->error_now=360-manner;
    }       
    else if(manner<180)
    {
        pid_jy901->error_now=error;
    }
    // if(myabs(pid_jy901->error_now)<0.5)
    //     pid_jy901->error_now=0;
    pid_jy901->error_integral=pid_jy901->error_integral+pid_jy901->error_now;
    float P=pid_jy901->kp*pid_jy901->error_now;
    float I=pid_jy901->ki*pid_jy901->error_integral;
    float D=pid_jy901->kd*(pid_jy901->error_now-pid_jy901->error_last);
    pid_jy901->output=P+I+D;
			encoder_l.position.distance=(encoder_l.position.distance+encoder_r.position.distance)/2;
			encoder_r.position.distance=encoder_l.position.distance;
//		{
			if(distance>=0)
			{	
				IT_PIDapplication_keep_distance(&s_pid_l,&v_pid_l,&encoder_l,distance,LEFTTYRE,v_target-pid_jy901->output);
				IT_PIDapplication_keep_distance(&s_pid_r,&v_pid_r,&encoder_r,distance,RIGHTTYRE,v_target+pid_jy901->output);

			}
			else if(distance<0)
			{
				IT_PIDapplication_keep_distance(&s_pid_l,&v_pid_l,&encoder_l,distance,LEFTTYRE,v_target+pid_jy901->output);
				IT_PIDapplication_keep_distance(&s_pid_r,&v_pid_r,&encoder_r,distance,RIGHTTYRE,v_target-pid_jy901->output);
			}
//		}
				

}

/*陀螺仪差速走固定速度,增量式*/
void go_straight_jy901_keepvelocity(PID_JY901* pid_jy901,const float target,const float v_target)
{
		const float kp=pid_jy901->kp;
		const float ki=pid_jy901->ki;
		const float kd=pid_jy901->kd;
	  int manner,error;   
    pid_jy901->target=target;
    pid_jy901->error_last=pid_jy901->error_now;
    error=target-Yaw;
    manner=abs_int(error);
    if(manner>=180)
    {
        if(error>=0)
            pid_jy901->error_now=-(360-manner);
        else if(error<0)
            pid_jy901->error_now=360-manner;
    }       
    else if(manner<180)
    {
        pid_jy901->error_now=error;
    }
    // if(myabs(pid_jy901->error_now)<0.5)
    //     pid_jy901->error_now=0;
    pid_jy901->error_integral=pid_jy901->error_integral+pid_jy901->error_now;
//    float P=kp*pid_jy901->error_now;
//    float I=ki*pid_jy901->error_integral;
//    float D=kd*(pid_jy901->error_now-pid_jy901->error_last);
		
    pid_jy901->output=kp*(pid_jy901->error_now-pid_jy901->error_last)+ki*pid_jy901->error_now+kd*(pid_jy901->error_now-2*pid_jy901->error_last+pid_jy901->error_lastlast);
		//pid_jy901->output=limit(pid_jy901->output,0.5*v_target);
    IT_PIDapplication_keep_velocity(&v_pid_l,&encoder_l,v_target-pid_jy901->output,LEFTTYRE);
    IT_PIDapplication_keep_velocity(&v_pid_r,&encoder_r,v_target+pid_jy901->output,RIGHTTYRE);
		//PWM_L=(int)(PWM_L-pid_jy901->output);
	  //PWM_R=(int)(PWM_R+pid_jy901->output);
		pid_jy901->error_lastlast=pid_jy901->error_last;
		pid_jy901->error_last=pid_jy901->error_now;

}

/*陀螺仪原地拐弯*/
void turn_angle_jy901(PID_JY901* pid_jy901,const float target)
{
	  int manner,error;   
    pid_jy901->target=target;
    pid_jy901->error_last=pid_jy901->error_now;
    error=target-Yaw;
    manner=abs_int(error);
    if(manner>=180)
    {
        if(error>=0)
            pid_jy901->error_now=-(360-manner);
        else if(error<0)
            pid_jy901->error_now=360-manner;
    }       
    else if(manner<180)
    {
        pid_jy901->error_now=error;
    }
    // if(myabs(pid_jy901->error_now)<0.5)
    //     pid_jy901->error_now=0;
    pid_jy901->error_integral=pid_jy901->error_integral+pid_jy901->error_now;
		pid_jy901->error_integral=limit(pid_jy901->error_integral,1200);
    float P=pid_jy901->kp*pid_jy901->error_now;
    float I=pid_jy901->ki*pid_jy901->error_integral;
    float D=pid_jy901->kd*(pid_jy901->error_now-pid_jy901->error_last);
    pid_jy901->output=P+I+D;
		pid_jy901->output=limit(pid_jy901->output,120);
		PWM_L=-pid_jy901->output;
	  PWM_R=pid_jy901->output;
}


/*编码器原地转弯,angle_line为正顺时针*/
void turn_angle_encoder(const float angle_line,const float chasu)
{
	IT_PIDapplication_keep_distance(&s_pid_l,&v_pid_l,&encoder_l,angle_line,LEFTTYRE,chasu);
	IT_PIDapplication_keep_distance(&s_pid_r,&v_pid_r,&encoder_r,-angle_line,RIGHTTYRE,chasu);
}

void stop(void)
{
	PWM_L=0;
	PWM_R=0;
}


/*************************************************步进电机***********************************************/

extern Stepper stepper_x;
extern Stepper stepper_y;

extern PID s_X_stepPID;
extern PID s_Y_stepPID;
extern PID v_X_stepPID;
extern PID v_Y_stepPID;

extern float F_Pulse_X;
extern float F_Pulse_Y;

//步进电机速度PID，位置式
void  Stepper_keep_velocity(Stepper* stepper,PID* v_pid,float v_target)
{
	set_PID_Target(v_pid,v_target);
	update_PID(v_pid,stepper->stepper_velocity_angle.now,positionPID,0,0);//λ��PID
	//printf("%f,%f,%f\n",v_pid->test,v_pid->input,v_pid->kp);
	if(stepper==&stepper_x)
	{
    F_Pulse_X=v_pid->output;
	}
	else if(stepper==&stepper_y)
	{
		F_Pulse_Y=v_pid->output;
	}
}

void  Stepper_keep_velocity_reserve(Stepper* stepper,PID* v_pid,float v_target)
{
	set_PID_Target(v_pid,v_target);
	update_PID(v_pid,stepper->stepper_velocity_angle.now,positionPID,0,0);//λ��PID
	//printf("%f,%f,%f\n",v_pid->test,v_pid->input,v_pid->kp);
	if(stepper==&stepper_x)
	{
    F_Pulse_X=-v_pid->output;
	}
	else if(stepper==&stepper_y)
	{
		F_Pulse_Y=-v_pid->output;
	}
}

//步进电机距离PID，位置式
void  Stepper_keep_distance(Stepper* stepper,PID* s_pid,float s_target,float v_limit)
{
	set_PID_Target(s_pid,s_target);
	update_PID(s_pid,stepper->stepper_distance_angle.now,positionPID,0,0);//λ��PID
	s_pid->output=limit(s_pid->output,v_limit);
	if(stepper==&stepper_x)
	{
    F_Pulse_X=s_pid->output;
	}
	else if(stepper==&stepper_y)
	{
		F_Pulse_Y=s_pid->output;
	}
}
//用于摄像头与云台绑在一起（目标为中心），单环距离
void  Stepper_keep_distance_reserve(Stepper* stepper,PID* s_pid,float s_target,float v_limit)
{
	set_PID_Target(s_pid,s_target);
	update_PID(s_pid,stepper->stepper_distance_angle.now,positionPID,0,0);//λ��PID
	s_pid->output=limit(s_pid->output,v_limit);
	if(stepper==&stepper_x)
	{
    F_Pulse_X=-s_pid->output;
	}
	else if(stepper==&stepper_y)
	{
		F_Pulse_Y=-s_pid->output;
	}
}

//双环PID,
void Stepper_keep_distance_Doublecircle(Stepper* stepper,PID* v_pid,PID* s_pid,float s_target,float v_target)
{
	set_PID_Target(s_pid,s_target);
	update_PID(s_pid,stepper->stepper_distance_angle.now,positionPID,0,0);
	s_pid->output=limit(s_pid->output,v_target);
  if((s_pid->output<=0.5f)&&(s_pid->output>=-0.5f))
        s_pid->output=0;
	if(count_project%2==0)
		Stepper_keep_velocity(stepper,v_pid,s_pid->output);
}

//双环PID，用于摄像头与云台绑在一起（目标为中心）
void Stepper_keep_distance_Doublecircle_reserve(Stepper* stepper,PID* v_pid,PID* s_pid,float s_target,float v_target)
{
	set_PID_Target(s_pid,s_target);
	update_PID(s_pid,stepper->stepper_distance_angle.now,positionPID,0,0);
	s_pid->output=limit(s_pid->output,v_target);
  if((s_pid->output<=0.5f)&&(s_pid->output>=-0.5f))
        s_pid->output=0;
	if(count_project%2==0)
  {
    Stepper_keep_velocity_reserve(stepper,v_pid,s_pid->output);
  }

}