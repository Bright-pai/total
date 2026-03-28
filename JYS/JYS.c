#include "ti_msp_dl_config.h"
#include "Delay.h"
#include "oled.h"
#include "UART.h"
#include "JYS.h"

extern float Yaw;
extern float Yaw_Last;
extern float Yaw_Add;

static int YawL,YawH,VL,VH,SUM;
// 串口接收状态标识
#define WAIT_HEADER1 0
#define WAIT_HEADER2 1
#define RECEIVE_DATA 2

static uint8_t RxState_101 = WAIT_HEADER1;
static uint8_t receivedData[9];
static uint8_t dataIndex = 0;


void JY61P_Enable(void)
{
	//NVIC_ClearPendingIRQ(UART1_INT_IRQn);
	NVIC_EnableIRQ(UART1_INT_IRQn);
}

void pid_JY61P_init_Param(PID_JY901* pid_jy901,const float kp,const float ki,const float kd)
{
	pid_jy901->kp=kp;
    pid_jy901->ki=ki;
    pid_jy901->kd=kd;
    pid_jy901->error_now=0;
    pid_jy901->error_last=0;
	pid_jy901->error_lastlast=0;
    pid_jy901->error_integral=0;
    pid_jy901->target=0;
    pid_jy901->output=0;

}

void pid_JY61P_init_zero(PID_JY901* pid_jy901)
{
	pid_jy901->error_now=0;
    pid_jy901->error_last=0;
	pid_jy901->error_lastlast=0;
    pid_jy901->error_integral=0;
    pid_jy901->target=0;
    pid_jy901->output=0;
}


void Serial_JY61P_Zero_Yaw(void){
   
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X76);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);

	
}

void serial_JY61P_zeroXY(void)
{
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X69);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X88);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XB5);
	delay_ms(200);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X01);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X08);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);
	delay_ms(3000);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);

}

void serial_JY61P_AUTO_UNSUAL(void)
{
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X69);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X88);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XB5);
	delay_ms(200);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X61);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X01);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);
	delay_ms(3000);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);

}

void serial_JY61P_SET_HZ(void)
{
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X69);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X88);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XB5);
	delay_ms(200);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X03);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0x0B);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);
	delay_ms(3000);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);

}

void serial_JY61P_OVERCPME_ZERO(void)
{
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X69);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X88);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XB5);
	delay_ms(200);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X07);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0xE8);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X03);
	delay_ms(3000);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);
	DL_UART_Main_transmitDataBlocking(UART_1_INST,0X00);

}

float X,Y,Z;
uint8_t Flag;
uint8_t jy901pak[9];




void UART_1_INST_IRQHandler(void)
{
    static uint8_t RState=0;
	static uint8_t Rpacket=0;
	uint8_t uart3_rxbuff;
	
	switch (DL_UART_Main_getPendingInterrupt(UART_1_INST)) 
	{
		case DL_UART_MAIN_IIDX_RX:
		{
		
			uart3_rxbuff = DL_UART_Main_receiveDataBlocking(UART_1_INST);				//读取数据寄存器，存放在接收的数据变量					
	
		
			switch (RxState_101) 
			{
				case WAIT_HEADER1:
				{
					if (uart3_rxbuff == 0x55) 
					{
						RxState_101 = WAIT_HEADER2;
					}
				}
				break;
				case WAIT_HEADER2:
				{
					if (uart3_rxbuff == 0x53) 
					{
					RxState_101 = RECEIVE_DATA;
					dataIndex = 0;
					} 
					else 
					{
						RxState_101 = WAIT_HEADER1; // 如果不是期望的第二个头，重置状态
					}
				}
				break;
				case RECEIVE_DATA:
				{			
					receivedData[dataIndex++] = uart3_rxbuff;
					if (dataIndex == 9) 
					{
						// 数据接收完毕，分配给具体的变量
						YawL = receivedData[4];
						YawH = receivedData[5];
						VL = receivedData[6];
						VH = receivedData[7];
						SUM = receivedData[8];
						// 校验SUM是否正确
						uint8_t calculatedSum = 0x55 + 0x53 +YawH + YawL + VH + VL;
						if (calculatedSum == SUM) 
						{
							//Yaw = (float)(((uint16_t)YawH << 8) | YawL)/32768*2000;
							if((float)(((uint16_t)YawH << 8) | YawL)/32768*180 >180)
							{
								Yaw = (float)(((uint16_t)YawH << 8) | YawL)/32768*180 - 360;
							}
							else   
							{
								Yaw = (float)(((uint16_t)YawH << 8) | YawL)/32768*180;
							}
							Yaw_Add=Yaw-Yaw_Last;
							Yaw_Last=Yaw;
							
							
						} 
						else 
						{
							// 校验失败，处理错误
						}

						RxState_101 = WAIT_HEADER1; // 重置状态以等待下一个数据包
					}

				}
				break;
			}
		}
		break;
		default:
			break;
	
	}
}

		