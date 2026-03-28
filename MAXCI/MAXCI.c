#include "MAXCI.h"

extern uint8_t Rx_Maxci_Data[9];
extern uint16_t Pixel_X;
extern uint16_t Pixel_Y;
extern int Rx_Maxci_TransferNum;
void MAXIC_RX_Num_Set(int num)
{

    Rx_Maxci_TransferNum=num;
}

void MAXIC_Enable(void)
{
	//NVIC_ClearPendingIRQ(UART1_INT_IRQn);
	//NVIC_EnableIRQ(UART0_INT_IRQn);

    DL_DMA_setSrcAddr(DMA, DMA_MAXCI_CHAN_ID , (uint32_t)(&UART_0_INST->RXDATA));
    DL_DMA_setDestAddr(DMA, DMA_MAXCI_CHAN_ID , (uint32_t)&Rx_Maxci_Data[0]);
    DL_DMA_setTransferSize(DMA,DMA_MAXCI_CHAN_ID ,Rx_Maxci_TransferNum);
    DL_DMA_enableChannel(DMA, DMA_MAXCI_CHAN_ID );

    NVIC_ClearPendingIRQ(UART0_INT_IRQn);
    NVIC_EnableIRQ(UART0_INT_IRQn);
}


// void Init_STP(void)
// {
//     DL_DMA_setSrcAddr(DMA, DMA_STP_CHAN_ID, (uint32_t)(&UART_3_INST->RXDATA));
//     DL_DMA_setDestAddr(DMA, DMA_STP_CHAN_ID, (uint32_t)&RX_Packet[0]);
//     DL_DMA_setTransferSize(DMA,DMA_STP_CHAN_ID,UART3_PACKET_SIZE);
//     DL_DMA_enableChannel(DMA, DMA_STP_CHAN_ID);

//     NVIC_ClearPendingIRQ(UART3_INT_IRQn);
//     NVIC_EnableIRQ(UART3_INT_IRQn);
// }
// void UART_0_INST_IRQHandler(void)
// {
//     uint8_t RxDate;
//     static uint8_t RState;
// 	static uint8_t Rpacket;
//     uint8_t Max_Flag;

//     switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
//         case DL_UART_IIDX_RX:
//             RxDate = DL_UART_receiveDataBlocking(UART_0_INST);
//             if (RState == 0) {
//                 if (RxDate == 97) {
//                     RState = 1;		
//                 }
//             }
//             else if (RState == 1) {
//                 if (RxDate == 100) {
//                     RState = 2;
//                     Rpacket = 0;
//                 }
//             }
//             else if (RState == 2) {
//                 Rx_Maxci_Data[Rpacket++] = RxDate - 48;
//                 if (Rpacket == 7) {
//                     RState = 3;
//                     Pixel_X=Rx_Maxci_Data[0]*100+Rx_Maxci_Data[1]*10+Rx_Maxci_Data[2];
//                     Pixel_Y=Rx_Maxci_Data[3]*100+Rx_Maxci_Data[4]*10+Rx_Maxci_Data[5];
//                 }
//             }
//             else if (RState == 3) {
//                 Max_Flag = 1;
//             }
//             break;
//         default:
//             break;  
//     }
// }