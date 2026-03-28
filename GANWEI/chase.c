#include "chase.h"
#include "Key.h"
#include <string.h>
#include "UART.h"
#include "Delay.h"
#include "Key.h"
#include "oled.h"
extern PID_TR_GW pid_tr;
extern volatile uint16_t oringinal_data[20];
extern volatile uint16_t anolag_result[8];
extern volatile uint16_t real_anolog_result[8];
extern uint16_t white[8];
extern uint16_t black[8];
extern uint16_t threshould[8];
extern uint16_t digital[8];
int Calibrate_flag=0;
extern volatile float ganweitr_error;
extern uint16_t status;

extern int count_project;
void tr_init_ganwei(PID_TR_GW* pid_tr,float kp,float ki,float  kd)
{
	pid_tr->kd=kd;
	pid_tr->ki=ki;
	pid_tr->kp=kp;
	pid_tr->error=0;
	pid_tr->last_error=0;
    pid_tr->tracestate=0;
}

int32_t pid_gawei_output( PID_TR_GW* pid_tr)
{
	int32_t P;
	static int32_t I;
	int32_t D;
	int32_t pid_output;
    switch(pid_tr->tracestate)
    {
    
        case 0X80: {pid_tr->error=-7;} break;
        case 0XC0: {pid_tr->error=-6;} break;
        case 0XE0: {pid_tr->error=-5.5;} break;
        case 0X40: {pid_tr->error=-5;} break;
        case 0X60: {pid_tr->error=-4;} break;
        case 0X70: {pid_tr->error=-3.5;} break;
        case 0X20: {pid_tr->error=-3;} break;
        case 0X30: {pid_tr->error=-2;} break;
        case 0X10: {pid_tr->error=-1;} break;
        case 0X08: {pid_tr->error=1;} break;
        case 0X0C: {pid_tr->error=2;} break;
        case 0X04: {pid_tr->error=3;} break;
        case 0X0E: {pid_tr->error=3.5;} break;
        case 0X06: {pid_tr->error=4;} break;
        case 0X02: {pid_tr->error=5;} break;
        case 0X07: {pid_tr->error=5.5;} break;
        case 0X03: {pid_tr->error=6;} break;
        case 0X01: {pid_tr->error=7;} break;
        case 0X18:	{pid_tr->error=0;} break;
        default:{pid_tr->error=0;} break;
    }
	pid_tr->P = pid_tr->kp * pid_tr->error;
	pid_tr->I +=pid_tr->ki * pid_tr->error;
	pid_tr->D = pid_tr->kd * ( pid_tr->error -pid_tr->last_error );
	pid_tr->last_error = pid_tr->error;
	pid_tr->output = (int32_t )(pid_tr->P + pid_tr->I + pid_tr->D);
    DL_ADC12_enableConversions(ADC_CHASE_INST);
	return pid_tr->output;
 
}


void CHASE_ADC_Init(void)
{
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &ADC_CHASE_INST->ULLMEM.MEMRES[0]);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &oringinal_data[0]);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
    NVIC_EnableIRQ(ADC_CHASE_INST_INT_IRQN);
    DL_ADC12_startConversion(ADC_CHASE_INST);
}

bool adc_getvalue(void)
{   
    // uint16_t gAdcReslt = 0 ;
    // DL_ADC12_enableConversions(ADC_CHASE_INST);
    // DL_ADC12_startConversion(ADC_CHASE_INST);
    // while (DL_ADC12_getStatus(ADC_CHASE_INST) == ADC12_STATUS_BUSY_IDLE);//如果当前状态 不是 空闲状态
    
    // DL_ADC12_stopConversion(ADC_CHASE_INST);
    // DL_ADC12_disableConversions(ADC_CHASE_INST);
    // gAdcReslt = DL_ADC12_getMemResult(ADC_CHASE_INST,ADC_CHASE_ADCMEM_0);
    DL_ADC12_disableConversions(ADC_CHASE_INST);
    static uint16_t ganwei_pos=0;
    bool ret=false;
    int sum=0;
    for(int i=0;i<20;i++)
    {
        sum=sum+oringinal_data[i];
    }
    anolag_result[ganwei_pos]=sum/20;
    //Serial_Printf("%d\n,",anolag_result[ganwei_pos]);
	if(ganwei_pos==7)
	{
        //Serial_Printf("\n");
		ganwei_pos=0;
        for(int i=0;i<8;i++)
        {
            real_anolog_result[i]=anolag_result[i];
        }
        ret=true;
	}
    else
    {
        ganwei_pos++;
        ret=false;
    }    
    Switch_Address_0(!(ganwei_pos&0x01));  // 地址线0，对应bit0
    Switch_Address_1(!(ganwei_pos&0x02));  // 地址线1，对应bit1
    Switch_Address_2(!(ganwei_pos&0x04));

    DL_ADC12_enableConversions(ADC_CHASE_INST);
    DL_ADC12_startConversion(ADC_CHASE_INST);
    return ret;   
}


void ADC_CHASE_INST_IRQHandler (void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC_CHASE_INST)) {
        case DL_ADC12_IIDX_DMA_DONE:
        {
            bool ret=adc_getvalue();
            uint16_t area_state=0;
            if((ret==true)&&(Calibrate_flag==1))
            {
                 for(int i=0;i<8;i++)
                 {
                    if(real_anolog_result[i]<threshould[i])
                        digital[i]=1;
                    else
                        digital[i]=0;
                    //Serial_Printf("%d,",digital[i]);
                    area_state|=( digital[i] << i );
                 }
                 pid_tr.tracestate=area_state;
                 //Serial_Printf("%d,%d\n",count_project,pid_tr.tracestate);
            }


            // DL_ADC12_disableConversions(ADC_CHASE_INST);
            // Serial_Printf("%d\n",anolag_result[0]);
        
            // //DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &M[ganwei_pos]);
            // // DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &ADC_CHASE_INST->ULLMEM.MEMRES[0]);
            // // DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&anolag_result[0]);
            // // DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
            // // NVIC_EnableIRQ(ADC_CHASE_INST_INT_IRQN);
            // DL_ADC12_enableConversions(ADC_CHASE_INST);
            // DL_ADC12_startConversion(ADC_CHASE_INST);

        }
            break;
        default:
            break;
    }
}

void ganwei_Calibrate(void)
{
    
    while(!(Key_GetNum()==KEY1))
    {}
    OLED_ShowString(0,4,(uint8_t*)"B",16);
    //Serial_Printf("start1\n");
    for(int i=0;i<8;i++)
    {
        black[i]=real_anolog_result[i];
        
    }
    Delay_s(1);
    while(!(Key_GetNum()==KEY1))
    {}
    OLED_ShowString(9,4,(uint8_t*)"W",16);
    //Serial_Printf("start2\n");
    for(int i=0;i<8;i++)
    {
        white[i]=real_anolog_result[i];
        threshould[i]=(white[i]+black[i])/2;
        //Serial_Printf("%d,",threshould[i]);
    }
    //Serial_Printf("\n");
    Calibrate_flag=1;
    //Delay_s(5);
}
