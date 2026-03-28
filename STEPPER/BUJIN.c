//32细分 6400一转  3200HZ
//SPEED = HZ/6400  HZ=32000000/ARR
#include "BUJIN.h"
#include <math.h>
#include "UART.h"


uint32_t X_NUM = 0;
uint32_t Y_NUM = 0;

uint8_t X_complete_Flag = 0;
uint8_t Y_complete_Flag = 0;

Stepper stepper_x;
Stepper stepper_y;

void Set_Bujin(void)
{
    DL_GPIO_setPins(GPIO_BUJIN_PIN_1EN_PORT,GPIO_BUJIN_PIN_1EN_PIN);
    DL_GPIO_setPins(GPIO_BUJIN_PIN_2EN_PORT,GPIO_BUJIN_PIN_2EN_PIN);
}

void Init_Bujin(void)
{
    DL_Timer_enableClock(PWM_BUJIN_X_INST);
    //NVIC_ClearPendingIRQ(PWM_BUJIN_X_INST_INT_IRQN);
    //NVIC_EnableIRQ(PWM_BUJIN_X_INST_INT_IRQN);

    DL_Timer_enableClock(PWM_BUJIN_Y_INST);
    //NVIC_ClearPendingIRQ(PWM_BUJIN_Y_INST_INT_IRQN);
    //NVIC_EnableIRQ(PWM_BUJIN_Y_INST_INT_IRQN);
}

void Start_X_Bujin(void)
{
    DL_Timer_startCounter(PWM_BUJIN_X_INST);

}

void Start_Y_Bujin(void)
{
    DL_Timer_startCounter(PWM_BUJIN_Y_INST);
}

void Stop_X_Bujin(void)
{    
    DL_Timer_stopCounter(PWM_BUJIN_X_INST);
}

void Stop_Y_Bujin(void)
{
    DL_Timer_stopCounter(PWM_BUJIN_Y_INST);
}

void Stepper_Init(Stepper* stepper_x,Stepper* stepper_y)
{
	stepper_x->stepper_distance_angle.now=0;
	stepper_x->stepper_distance_angle.last=0;
	stepper_x->stepper_velocity_angle.now=0;
	stepper_x->stepper_velocity_angle.last=0;
	
	stepper_y->stepper_distance_angle.now=0;
	stepper_y->stepper_distance_angle.last=0;
	stepper_y->stepper_velocity_angle.now=0;
	stepper_y->stepper_velocity_angle.last=0;
}

void Pixel_Calibration_To_Angle_Update(Stepper* stepper_x,Stepper* stepper_y,int x,int y,int x0,int y0)
{
	stepper_x->stepper_distance_angle.last=stepper_x->stepper_distance_angle.now;
	stepper_x->stepper_velocity_angle.last=stepper_x->stepper_velocity_angle.now;
	stepper_y->stepper_distance_angle.last=stepper_y->stepper_distance_angle.now;
	stepper_y->stepper_velocity_angle.last=stepper_y->stepper_velocity_angle.now;
	//printf("%d\n",x-x0);
	//stepper_x->stepper_distance_angle.now=180*atan2(x-x0,Place_Distance)/3.14159f;
	//stepper_y->stepper_distance_angle.now=180*atan2(y-y0,Place_Distance)/3.14159f;
	//printf("%f\n",stepper_x->stepper_distance_angle.now);
	stepper_x->stepper_distance_angle.now=x-x0;
	stepper_y->stepper_distance_angle.now=y-y0;
	stepper_x->stepper_velocity_angle.now=(stepper_x->stepper_distance_angle.now)-(stepper_x->stepper_distance_angle.last);
	stepper_y->stepper_velocity_angle.now=(stepper_y->stepper_distance_angle.now)-(stepper_y->stepper_distance_angle.last);
}

//0顺时针，1逆时针
//0.08-1 speed
void Set_X_Bujin_Speed(float Speed)
{   float Value;
    float FABSSPEED;
    if (Speed>=0) {
        FABSSPEED = Speed;
        DL_GPIO_setPins(GPIO_BUJIN_PIN_1DIR_PORT, GPIO_BUJIN_PIN_1DIR_PIN);
    }
    else {
        FABSSPEED = -Speed;
        DL_GPIO_clearPins(GPIO_BUJIN_PIN_1DIR_PORT, GPIO_BUJIN_PIN_1DIR_PIN);
    }
    //DL_Timer_stopCounter(PWM_BUJIN_X_INST);
    Value = 500000 / FABSSPEED ;
    // 限制最大值，避免超出定时器范围
    if (Value > 0xFFFF) {
        Value = 0xFFFF;
    }
    DL_Timer_setLoadValue(PWM_BUJIN_X_INST, Value);
    DL_Timer_setCaptureCompareValue(PWM_BUJIN_X_INST,Value/2,DL_TIMER_CC_1_INDEX);
    if(Speed!=0)
        Start_X_Bujin();
    else 
        DL_Timer_stopCounter(PWM_BUJIN_X_INST);
   // DL_Timer_startCounter(PWM_BUJIN_X_INST);
}

void Set_Y_Bujin_Speed(float Speed)
{   float Value;
    float FABSSPEED;
    if (Speed>=0) {
        FABSSPEED = Speed;
        DL_GPIO_clearPins(GPIO_BUJIN_PIN_2DIR_PORT, GPIO_BUJIN_PIN_2DIR_PIN);
    }
    else 
    {
        FABSSPEED = -Speed;
        DL_GPIO_setPins(GPIO_BUJIN_PIN_2DIR_PORT, GPIO_BUJIN_PIN_2DIR_PIN);
    }
    //DL_Timer_stopCounter(PWM_BUJIN_Y_INST);
    Value = 500000 / FABSSPEED ;
    // 限制最大值，避免超出定时器范围
    if (Value > 0xFFFF) {
        Value = 0xFFFF;
    }
    DL_Timer_setLoadValue(PWM_BUJIN_Y_INST, Value);
    DL_Timer_setCaptureCompareValue(PWM_BUJIN_Y_INST,Value/2,DL_TIMER_CC_3_INDEX);
    if(Speed!=0)
        Start_Y_Bujin();
    else 
        DL_Timer_stopCounter(PWM_BUJIN_Y_INST);
}
//0顺1逆
void Bujin_Set_X_Num(uint32_t Nuum)
{
    X_NUM = Nuum;    
}
void Bujin_Set_Y_Num(uint32_t Nuum)
{
    Y_NUM = Nuum;    
}

void PWM_BUJIN_X_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(PWM_BUJIN_X_INST)) {
        case DL_TIMER_IIDX_ZERO:
            
            if (X_NUM <= 0) {
                DL_Timer_stopCounter(PWM_BUJIN_X_INST);
                X_complete_Flag = 0;
            }    
            else {
                 X_NUM--; 
            }
          //  X_NUM--;        
           break;
        default:
           break;
    }
}
 
void PWM_BUJIN_Y_INST_IRQHandler()
{
    switch (DL_TimerA_getPendingInterrupt(PWM_BUJIN_Y_INST)) {
        case DL_TIMER_IIDX_ZERO:
                            
                if (Y_NUM <= 0) {
                    DL_TimerA_stopCounter(PWM_BUJIN_Y_INST);
                    Y_complete_Flag = 0;
                }    
                else {
                    Y_NUM--; 
                }    
              //  Y_NUM--;    
            break;           
        default:
            break;
    }
}


