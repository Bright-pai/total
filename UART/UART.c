#include "ti_msp_dl_config.h"
#include "UART.h"
#include <stdarg.h>
#include <stdio.h>
#include "Delay.h"



uint8_t Serial_TxPacket[6];				//定义发送数据包数组，数据包格式：FF 01 02 03 04 FE
uint8_t Serial_RxFlag;					//定义接收数据包标志位


/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Serial_SendByte(uint8_t Byte)
{	
    while(DL_UART_isBusy(UART_0_INST)==1);
    DL_UART_Main_transmitDataBlocking(UART_0_INST,Byte);
}

/* 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		Serial_SendByte(Array[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Serial_SendByte(String[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

/**
  * 函    数：次方函数（内部使用）
  * 返 回 值：返回值等于X的Y次方
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//设置结果初值为1
	while (Y --)			//执行Y次
	{
		Result *= X;		//将X累乘到结果
	}
	return Result;
}

/**
  * 函    数：串口发送数字
  * 参    数：Number 要发送的数字，范围：0~4294967295
  * 参    数：Length 要发送数字的长度，范围：0~10
  * 返 回 值：无
  */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//根据数字长度遍历数字的每一位
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');	//依次调用Serial_SendByte发送每位数字
	}
}

/**
  * 函    数：获取串口接收标志位
  * 参    数：无
  * 返 回 值：串口接收标志位，范围：0~1，接收到数据后，标志位置1，读取后标志位自动清零
  */
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)			//如果标志位为1
	{
		Serial_RxFlag = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}

uint8_t Serial_RxPacket[4];
/*
void UART_0_INST_IRQHandler(void)
{   static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
	static uint8_t pRxPacket = 0;	//定义表示当前接收数据位置的静态变
	uint8_t RxData;
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
        RxData = DL_UART_Main_receiveDataBlocking(UART_0_INST);				//读取数据寄存器，存放在接收的数据变量
	*/	
		/*使用状态机的思路，依次处理数据包的不同部分*/
		
		/*当前状态为0，接收数据包包头*/
	/*	if (RxState == 0)
		{
			if (RxData == 0xFF)			//如果数据确实是包头
			{
				RxState = 1;			//置下一个状态
				pRxPacket = 0;			//数据包的位置归零
			}
		}*/
		/*当前状态为1，接收数据包数据*/
	/*	else if (RxState == 1)
		{
			Serial_RxPacket[pRxPacket] = RxData;	//将数据存入数据包数组的指定位置
			pRxPacket ++;				//数据包的位置自增
			if (pRxPacket >= 4)			//如果收够4个数据
			{
				RxState = 2;			//置下一个状态
			}
		}*/
		/*当前状态为2，接收数据包包尾*/
/*	else if (RxState == 2)
		{
			if (RxData == 0xFE)			//如果数据确实是包尾部
			{
				RxState = 0;			//状态归0
				Serial_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
			}
		}
		
            break;
        default:
            break;
    }
}
*/
/**
  * 函    数：串口发送数据包
  * 参    数：无
  * 返 回 值：无
  * 说    明：调用此函数后，Serial_TxPacket数组的内容将加上包头（FF）包尾（FE）后，作为数据包发送出去
  */
void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);
	Serial_SendArray(Serial_TxPacket, 6);
	Serial_SendByte(0xFE);
}

/**
  * 函    数：使用printf需要重定向的底层函数
  * 参    数：保持原始格式即可，无需变动
  * 返 回 值：保持原始格式即可，无需变动
  */
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);			//将printf的底层重定向到自己的发送字节函数
	return ch;
}
/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void Serial_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	Serial_SendString(String);		//串口发送字符数组（字符串）
}
/*
float X,Y,Z;
uint8_t Flag;
uint8_t jy901pak[9];
void UART_1_INST_IRQHandler(void)
{
    static uint8_t RState;
	static uint8_t Rpacket;
	uint8_t RxDate;
	uint8_t Sum;
	switch (DL_UART_Main_getPendingInterrupt(UART_1_INST)) {
		case DL_UART_MAIN_IIDX_RX:
		
		RxDate = DL_UART_Main_receiveDataBlocking(UART_1_INST);				//读取数据寄存器，存放在接收的数据变量					
	
		
		if (RState == 0)
		{
			if (RxDate == 0x55)		
			{
				RState = 1;		
				;		
			}
		}
		else if (RState==1) {
             if (RxDate == 0x53)		
			{  
				RState = 2;			
				Rpacket = 0;
				Sum=0;		
			}
		
		}
		else if (RState==2) {			
			if (Rpacket<=7) {
			 Sum=jy901pak[Rpacket]+Sum;
			}
            jy901pak[Rpacket++]=RxDate;		
			if (Rpacket==9) {
			RState=3;
			}
		
		}
		else if (RState==3) {
	//		 X=((jy901pak[1]<<8)|jy901pak[0])/32768*180;
     //        Y=((jy901pak[3]<<8)|jy901pak[2])/32768*180;
     //        Z=((jy901pak[5]<<8)|jy901pak[4])/32768*180;
		if (Sum+0x55+0x53==jy901pak[8]) {
			   Flag=1;
			}
			else {
			   Flag=0;
			}
		    RState=0;
		}
            break;
		default:
		    break;
	
	}
}
uint8_t Get_Jy_Flag(void)
{
	if (Flag==1) {
	Flag=0;
	return 1;
	}
	else {
	return 0;
	}
}
*/


