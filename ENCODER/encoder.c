#include "encoder.h"
#include "ti_msp_dl_config.h"
/*���������ٶ�55,���������ٶ�53*/
void TIMER_IT_ENABLE(void)
{
	NVIC_ClearPendingIRQ(motor_CLOCK_INST_INT_IRQN);
    NVIC_EnableIRQ(motor_CLOCK_INST_INT_IRQN);
    DL_Timer_enableClock(motor_CLOCK_INST);
    DL_Timer_startCounter(motor_CLOCK_INST);

}

void TIMER_IT_DISENABLE(void)
{	
	NVIC_DisableIRQ(motor_CLOCK_INST_INT_IRQN);
    DL_Timer_disableClock(motor_CLOCK_INST);
	
}

void encoder_Init(Encoder* encoder_l,Encoder* encoder_r)
{

	NVIC_ClearPendingIRQ(Encoder1A_INST_INT_IRQN);
    NVIC_EnableIRQ(Encoder1A_INST_INT_IRQN);

    NVIC_ClearPendingIRQ(Encoder2A_INST_INT_IRQN);
    NVIC_EnableIRQ(Encoder2A_INST_INT_IRQN);


    DL_Timer_enableClock(Encoder1A_INST);
    DL_Timer_startCounter(Encoder1A_INST);
  

    DL_Timer_enableClock(Encoder2A_INST);
    DL_Timer_startCounter(Encoder2A_INST);


	encoder_r->count.count_last=0;
	encoder_r->count.count_now=0;
	encoder_r->count.count_total=0;
	encoder_r->position.angle=0;
	encoder_r->position.distance=0;
	encoder_r->position.rotations=0;
	encoder_r->velocity.angular=0;
	encoder_r->velocity.linear=0;
	encoder_l->count.count_last=0;
	encoder_l->count.count_now=0;
	encoder_l->count.count_total=0;
	encoder_l->position.angle=0;
	encoder_l->position.distance=0;
	encoder_l->position.rotations=0;
	encoder_l->velocity.angular=0;
	encoder_l->velocity.linear=0;
	

}


void Init_Zero_Encoder(Encoder* encoder_l,Encoder* encoder_r)
{
	encoder_r->count.count_last=0;
	encoder_r->count.count_now=0;
	encoder_r->count.count_total=0;
	encoder_r->position.angle=0;
	encoder_r->position.distance=0;
	encoder_r->position.rotations=0;
	encoder_r->velocity.angular=0;
	encoder_r->velocity.linear=0;
	encoder_l->count.count_last=0;
	encoder_l->count.count_now=0;
	encoder_l->count.count_total=0;
	encoder_l->position.angle=0;
	encoder_l->position.distance=0;
	encoder_l->position.rotations=0;
	encoder_l->velocity.angular=0;
	encoder_l->velocity.linear=0;

}

void encoder_update(Encoder* encoder_l,Encoder* encoder_r,const int count_l,const int count_r)
{
			encoder_l->count.count_last=encoder_l->count.count_now;
			encoder_r->count.count_last=encoder_r->count.count_now;
			encoder_l->count.count_now=(int32_t)count_l;
			encoder_r->count.count_now=(int32_t)count_r;
			encoder_l->count.count_total+=(int64_t)encoder_l->count.count_now;
			encoder_r->count.count_total+=(int64_t)encoder_r->count.count_now;
	
			encoder_l->velocity.linear=count_l*Timer_IT_Hz*CMperPAUSE ;
			encoder_r->velocity.linear=count_r*Timer_IT_Hz*CMperPAUSE ;
			encoder_l->velocity.angular=count_l*Timer_IT_Hz*ANGLEperPAUSE ;
			encoder_r->velocity.angular=count_r*Timer_IT_Hz*ANGLEperPAUSE;
	
			encoder_l->position.rotations=encoder_l->count.count_total/780;
			encoder_r->position.rotations=encoder_r->count.count_total/780;
			encoder_l->position.angle=encoder_l->position.rotations*360;
			encoder_r->position.angle=encoder_r->position.rotations*360;
			encoder_l->position.distance=encoder_l->count.count_total*CMperPAUSE;
			encoder_r->position.distance=encoder_r->count.count_total*CMperPAUSE;

			
}
