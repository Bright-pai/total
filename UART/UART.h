#ifndef _UART_H
#define _UART_H

void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
uint8_t Serial_GetRxFlag(void);
//uint8_t Get_Jy_Flag(void);

void Serial_SendPacket(void);
void Serial_Printf(char *format, ...);

//extern uint8_t jy901pak[];


#endif