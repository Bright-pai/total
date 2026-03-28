#include "pid.h"


float limit(float input,const float MAX_OUTPUT)//λ��˫����תʱ�ٶ�Ϊ����ֵ��ԭ��
{
	if(input>=0)
		input=input>+MAX_OUTPUT?+MAX_OUTPUT:input;
	else if(input<0)
		input=input<(-MAX_OUTPUT)?(-MAX_OUTPUT):input;
	return input;
}

/*����PID��������*/
void Reset_PID(PID* pid)
{

	//error
	pid->error.integral=0;
	pid->error.last=0;
	pid->error.now=0;
    pid->error.lastlast=0;
	//input,output
	pid->output=0;
	pid->target=0;	
}

/*
pid�޷�
*/
void set_PID_Limit(PID* pid,const float MAX_OUTPUT,const float MAX_ERROR_INTEGRAL)//?????
{
	pid->MAX_ERROR_INTEGRAL=MAX_ERROR_INTEGRAL;
	pid->MAX_OUTPUT=MAX_OUTPUT;
	
}

void init_zero_PID(PID* pid)
{
	pid->error.integral=0;
	pid->error.last=0;
	pid->error.now=0;
  pid->error.lastlast=0;
	//input,output
	pid->output=0;
	pid->target=0;
}
/*
mode=0:����ʽ
mode=1:λ��ʽ
ֱ�������λ��ʽ���������������ʽ��ֱ����pid->output��
*/

void update_PID(PID* pid,float input,PID_Mode mode,int inte,float limit_num )
{
  pid->input=input;
	pid->error.now=pid->target-pid->input;
//	if(pid->error.now<=0.5)
//		pid->error.now=0;
  if(mode==incrementalPid)
  {
    pid->output+=pid->kp*(pid->error.now-pid->error.last)+pid->ki*pid->error.now+pid->kd*(pid->error.now-2*pid->error.last+pid->error.lastlast);
    pid->output=limit(pid->output,pid->MAX_OUTPUT);

  }
  else if(mode==positionPID )
  {
    pid->error.integral+=pid->error.now;
		if((pid->error.now>limit_num)&&(inte==1))
			pid->error.integral=0;
		pid->error.integral =limit(pid->error.integral,pid->MAX_ERROR_INTEGRAL);
		pid->output=pid->kp*pid->error.now+pid->ki*pid->error.integral+pid->kd*(pid->error.now-pid->error.last);
		pid->test=pid->output;
    pid->output=limit(pid->output,pid->MAX_OUTPUT);
  }
  pid->error.lastlast=pid->error.last;
	pid->error.last=pid->error.now;
 
}

void set_PID_Param(PID* pid,float kp,float ki,float kd)
{
	pid->kp=kp;
	pid->ki=ki;
	pid->kd=kd;
}	

void set_PID_Target(PID* pid,float target)
{
	pid->target=target;
}

