#include "ti_msp_dl_config.h"
#include "STP.h"
#include "UART.h"
#include "oled.h"

uint8_t RX_Packet[UART3_PACKET_SIZE];
uint8_t ERR_BUFF[16];
uint16_t Rx_Stp_Data[11];
uint8_t Rx_Flag = 0;
void Init_STP(void)
{
    DL_DMA_setSrcAddr(DMA, DMA_STP_CHAN_ID, (uint32_t)(&UART_3_INST->RXDATA));
    DL_DMA_setDestAddr(DMA, DMA_STP_CHAN_ID, (uint32_t)&RX_Packet[0]);
    DL_DMA_setTransferSize(DMA,DMA_STP_CHAN_ID,UART3_PACKET_SIZE);
    DL_DMA_enableChannel(DMA, DMA_STP_CHAN_ID);

    NVIC_ClearPendingIRQ(UART3_INT_IRQn);
    NVIC_EnableIRQ(UART3_INT_IRQn);
}
/*
float Cal_Stp(void)
{   uint8_t i;
    float Sum = 0;
    if (Rx_Flag == 1) {
        for (i=0,Sum =0; i<12; i++) {
            Rx_Stp_Data[i] = (uint16_t)RX_Packet[10+15*i] + ((uint16_t)RX_Packet[10+15*i + 1]<<8);
            Sum = Sum + Rx_Stp_Data[i];
        }   
        Sum = Sum/12.0;
        SYSCFG_DL_DMA_init();
        DL_DMA_setSrcAddr(DMA, DMA_STP_CHAN_ID, (uint32_t)(&UART_3_INST->RXDATA));
        DL_DMA_setDestAddr(DMA, DMA_STP_CHAN_ID, (uint32_t)&RX_Packet[0]);
        DL_DMA_setTransferSize(DMA,DMA_STP_CHAN_ID,UART3_PACKET_SIZE);
        DL_DMA_enableChannel(DMA, DMA_STP_CHAN_ID);
        Rx_Flag = 0;
        return Sum;
    }
}
*/
void UART_3_INST_IRQHandler(void)
{   uint8_t ERRO;
    switch (DL_UART_Main_getPendingInterrupt(UART_3_INST)) {
        case DL_UART_IIDX_DMA_DONE_RX:
            DL_DMA_disableChannel(DMA, DMA_STP_CHAN_ID);
                if (!DL_UART_isRXFIFOEmpty(UART_3_INST)) {
                    ERRO = DL_UART_receiveData(UART_3_INST);
                    DL_DMA_setSrcAddr(DMA, DMA_STP_CHAN_ID, (uint32_t)(&UART_3_INST->RXDATA));
                    DL_DMA_setDestAddr(DMA, DMA_STP_CHAN_ID, (uint32_t)&RX_Packet[0]);
                    DL_DMA_setTransferSize(DMA,DMA_STP_CHAN_ID,UART3_PACKET_SIZE);
                    Rx_Flag = 0;
                }
                else if (RX_Packet[0] == 0XAA && RX_Packet[3] == 0XAA && RX_Packet[4] == 0X00) 
                {
                    Rx_Flag = 1;
                    uint8_t i;
                    uint16_t Sum = 0;
                    if (Rx_Flag == 1) {
                        for (i=0,Sum =0; i<12; i++) {
                            Rx_Stp_Data[i] = (uint16_t)RX_Packet[10+15*i] + ((uint16_t)RX_Packet[10+15*i + 1]<<8);
                            Sum = Sum + Rx_Stp_Data[i];
                        }   
                        Sum = Sum/12.0;
                        SYSCFG_DL_DMA_init();
                        DL_DMA_setSrcAddr(DMA, DMA_STP_CHAN_ID, (uint32_t)(&UART_3_INST->RXDATA));
                        DL_DMA_setDestAddr(DMA, DMA_STP_CHAN_ID, (uint32_t)&RX_Packet[0]);
                        DL_DMA_setTransferSize(DMA,DMA_STP_CHAN_ID,UART3_PACKET_SIZE);
                        DL_DMA_enableChannel(DMA, DMA_STP_CHAN_ID);
                        Rx_Flag = 0;
                        //Serial_Printf("%d\r\n",Sum);
                        OLED_ShowNum(10, 0, Sum, 4, 16);
                        }
                }
                else {
                    SYSCFG_DL_DMA_init();
                    DL_DMA_setSrcAddr(DMA, DMA_STP_CHAN_ID, (uint32_t)(&UART_3_INST->RXDATA));
                    DL_DMA_setDestAddr(DMA, DMA_STP_CHAN_ID, (uint32_t)&RX_Packet[0]);
                    DL_DMA_setTransferSize(DMA,DMA_STP_CHAN_ID,UART3_PACKET_SIZE);
                    DL_DMA_enableChannel(DMA, DMA_STP_CHAN_ID);
                    Rx_Flag = 0;
                }
                break;
            default:
                break;           
    }
}
