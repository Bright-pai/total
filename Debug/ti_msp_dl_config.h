/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for Motor */
#define Motor_INST                                                         TIMA1
#define Motor_INST_IRQHandler                                   TIMA1_IRQHandler
#define Motor_INST_INT_IRQN                                     (TIMA1_INT_IRQn)
#define Motor_INST_CLK_FREQ                                              1000000
/* GPIO defines for channel 0 */
#define GPIO_Motor_C0_PORT                                                 GPIOB
#define GPIO_Motor_C0_PIN                                          DL_GPIO_PIN_4
#define GPIO_Motor_C0_IOMUX                                      (IOMUX_PINCM17)
#define GPIO_Motor_C0_IOMUX_FUNC                     IOMUX_PINCM17_PF_TIMA1_CCP0
#define GPIO_Motor_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_Motor_C1_PORT                                                 GPIOB
#define GPIO_Motor_C1_PIN                                          DL_GPIO_PIN_5
#define GPIO_Motor_C1_IOMUX                                      (IOMUX_PINCM18)
#define GPIO_Motor_C1_IOMUX_FUNC                     IOMUX_PINCM18_PF_TIMA1_CCP1
#define GPIO_Motor_C1_IDX                                    DL_TIMER_CC_1_INDEX

/* Defines for PWM_BUJIN_X */
#define PWM_BUJIN_X_INST                                                   TIMG7
#define PWM_BUJIN_X_INST_IRQHandler                             TIMG7_IRQHandler
#define PWM_BUJIN_X_INST_INT_IRQN                               (TIMG7_INT_IRQn)
#define PWM_BUJIN_X_INST_CLK_FREQ                                         500000
/* GPIO defines for channel 1 */
#define GPIO_PWM_BUJIN_X_C1_PORT                                           GPIOB
#define GPIO_PWM_BUJIN_X_C1_PIN                                   DL_GPIO_PIN_19
#define GPIO_PWM_BUJIN_X_C1_IOMUX                                (IOMUX_PINCM45)
#define GPIO_PWM_BUJIN_X_C1_IOMUX_FUNC               IOMUX_PINCM45_PF_TIMG7_CCP1
#define GPIO_PWM_BUJIN_X_C1_IDX                              DL_TIMER_CC_1_INDEX

/* Defines for PWM_BUJIN_Y */
#define PWM_BUJIN_Y_INST                                                   TIMA0
#define PWM_BUJIN_Y_INST_IRQHandler                             TIMA0_IRQHandler
#define PWM_BUJIN_Y_INST_INT_IRQN                               (TIMA0_INT_IRQn)
#define PWM_BUJIN_Y_INST_CLK_FREQ                                         500000
/* GPIO defines for channel 3 */
#define GPIO_PWM_BUJIN_Y_C3_PORT                                           GPIOA
#define GPIO_PWM_BUJIN_Y_C3_PIN                                   DL_GPIO_PIN_25
#define GPIO_PWM_BUJIN_Y_C3_IOMUX                                (IOMUX_PINCM55)
#define GPIO_PWM_BUJIN_Y_C3_IOMUX_FUNC               IOMUX_PINCM55_PF_TIMA0_CCP3
#define GPIO_PWM_BUJIN_Y_C3_IDX                              DL_TIMER_CC_3_INDEX



/* Defines for Encoder1A */
#define Encoder1A_INST                                                   (TIMG6)
#define Encoder1A_INST_IRQHandler                               TIMG6_IRQHandler
#define Encoder1A_INST_INT_IRQN                                 (TIMG6_INT_IRQn)
#define Encoder1A_INST_LOAD_VALUE                                       (31999U)
/* GPIO defines for channel 1 */
#define GPIO_Encoder1A_C1_PORT                                             GPIOB
#define GPIO_Encoder1A_C1_PIN                                     DL_GPIO_PIN_27
#define GPIO_Encoder1A_C1_IOMUX                                  (IOMUX_PINCM58)
#define GPIO_Encoder1A_C1_IOMUX_FUNC                 IOMUX_PINCM58_PF_TIMG6_CCP1

/* Defines for Encoder2A */
#define Encoder2A_INST                                                   (TIMG8)
#define Encoder2A_INST_IRQHandler                               TIMG8_IRQHandler
#define Encoder2A_INST_INT_IRQN                                 (TIMG8_INT_IRQn)
#define Encoder2A_INST_LOAD_VALUE                                       (31999U)
/* GPIO defines for channel 0 */
#define GPIO_Encoder2A_C0_PORT                                             GPIOB
#define GPIO_Encoder2A_C0_PIN                                      DL_GPIO_PIN_6
#define GPIO_Encoder2A_C0_IOMUX                                  (IOMUX_PINCM23)
#define GPIO_Encoder2A_C0_IOMUX_FUNC                 IOMUX_PINCM23_PF_TIMG8_CCP0





/* Defines for motor_CLOCK */
#define motor_CLOCK_INST                                                 (TIMG0)
#define motor_CLOCK_INST_IRQHandler                             TIMG0_IRQHandler
#define motor_CLOCK_INST_INT_IRQN                               (TIMG0_INT_IRQn)
#define motor_CLOCK_INST_LOAD_VALUE                                      (9999U)




/* Defines for I2C_1 */
#define I2C_1_INST                                                          I2C1
#define I2C_1_INST_IRQHandler                                    I2C1_IRQHandler
#define I2C_1_INST_INT_IRQN                                        I2C1_INT_IRQn
#define GPIO_I2C_1_SDA_PORT                                                GPIOA
#define GPIO_I2C_1_SDA_PIN                                        DL_GPIO_PIN_30
#define GPIO_I2C_1_IOMUX_SDA                                      (IOMUX_PINCM5)
#define GPIO_I2C_1_IOMUX_SDA_FUNC                       IOMUX_PINCM5_PF_I2C1_SDA
#define GPIO_I2C_1_SCL_PORT                                                GPIOA
#define GPIO_I2C_1_SCL_PIN                                        DL_GPIO_PIN_29
#define GPIO_I2C_1_IOMUX_SCL                                      (IOMUX_PINCM4)
#define GPIO_I2C_1_IOMUX_SCL_FUNC                       IOMUX_PINCM4_PF_I2C1_SCL


/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           32000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_31
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_28
#define GPIO_UART_0_IOMUX_RX                                      (IOMUX_PINCM6)
#define GPIO_UART_0_IOMUX_TX                                      (IOMUX_PINCM3)
#define GPIO_UART_0_IOMUX_RX_FUNC                       IOMUX_PINCM6_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                       IOMUX_PINCM3_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_32_MHZ_115200_BAUD                                      (17)
#define UART_0_FBRD_32_MHZ_115200_BAUD                                      (23)
/* Defines for UART_1 */
#define UART_1_INST                                                        UART1
#define UART_1_INST_FREQUENCY                                           32000000
#define UART_1_INST_IRQHandler                                  UART1_IRQHandler
#define UART_1_INST_INT_IRQN                                      UART1_INT_IRQn
#define GPIO_UART_1_RX_PORT                                                GPIOA
#define GPIO_UART_1_TX_PORT                                                GPIOA
#define GPIO_UART_1_RX_PIN                                         DL_GPIO_PIN_9
#define GPIO_UART_1_TX_PIN                                         DL_GPIO_PIN_8
#define GPIO_UART_1_IOMUX_RX                                     (IOMUX_PINCM20)
#define GPIO_UART_1_IOMUX_TX                                     (IOMUX_PINCM19)
#define GPIO_UART_1_IOMUX_RX_FUNC                      IOMUX_PINCM20_PF_UART1_RX
#define GPIO_UART_1_IOMUX_TX_FUNC                      IOMUX_PINCM19_PF_UART1_TX
#define UART_1_BAUD_RATE                                                  (9600)
#define UART_1_IBRD_32_MHZ_9600_BAUD                                       (208)
#define UART_1_FBRD_32_MHZ_9600_BAUD                                        (21)
/* Defines for UART_3 */
#define UART_3_INST                                                        UART3
#define UART_3_INST_FREQUENCY                                           32000000
#define UART_3_INST_IRQHandler                                  UART3_IRQHandler
#define UART_3_INST_INT_IRQN                                      UART3_INT_IRQn
#define GPIO_UART_3_RX_PORT                                                GPIOB
#define GPIO_UART_3_TX_PORT                                                GPIOB
#define GPIO_UART_3_RX_PIN                                        DL_GPIO_PIN_13
#define GPIO_UART_3_TX_PIN                                        DL_GPIO_PIN_12
#define GPIO_UART_3_IOMUX_RX                                     (IOMUX_PINCM30)
#define GPIO_UART_3_IOMUX_TX                                     (IOMUX_PINCM29)
#define GPIO_UART_3_IOMUX_RX_FUNC                      IOMUX_PINCM30_PF_UART3_RX
#define GPIO_UART_3_IOMUX_TX_FUNC                      IOMUX_PINCM29_PF_UART3_TX
#define UART_3_BAUD_RATE                                                (230400)
#define UART_3_IBRD_32_MHZ_230400_BAUD                                       (8)
#define UART_3_FBRD_32_MHZ_230400_BAUD                                      (44)





/* Defines for ADC_CHASE */
#define ADC_CHASE_INST                                                      ADC0
#define ADC_CHASE_INST_IRQHandler                                ADC0_IRQHandler
#define ADC_CHASE_INST_INT_IRQN                                  (ADC0_INT_IRQn)
#define ADC_CHASE_ADCMEM_0                                    DL_ADC12_MEM_IDX_0
#define ADC_CHASE_ADCMEM_0_REF                   DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC_CHASE_ADCMEM_0_REF_VOLTAGE_V                                     3.3
#define GPIO_ADC_CHASE_C5_PORT                                             GPIOB
#define GPIO_ADC_CHASE_C5_PIN                                     DL_GPIO_PIN_24



/* Defines for DMA_CH0 */
#define DMA_CH0_CHAN_ID                                                      (1)
#define ADC_CHASE_INST_DMA_TRIGGER                    (DMA_ADC0_EVT_GEN_BD_TRIG)
/* Defines for DMA_MAXCI */
#define DMA_MAXCI_CHAN_ID                                                    (2)
#define UART_0_INST_DMA_TRIGGER                              (DMA_UART0_RX_TRIG)
/* Defines for DMA_STP */
#define DMA_STP_CHAN_ID                                                      (0)
#define UART_3_INST_DMA_TRIGGER                              (DMA_UART3_RX_TRIG)


/* Port definition for Pin Group GPIO_STBY */
#define GPIO_STBY_PORT                                                   (GPIOB)

/* Defines for PIN_1: GPIOB.7 with pinCMx 24 on package pin 59 */
#define GPIO_STBY_PIN_1_PIN                                      (DL_GPIO_PIN_7)
#define GPIO_STBY_PIN_1_IOMUX                                    (IOMUX_PINCM24)
/* Port definition for Pin Group MOTOR_IN */
#define MOTOR_IN_PORT                                                    (GPIOB)

/* Defines for AIN1: GPIOB.15 with pinCMx 32 on package pin 3 */
#define MOTOR_IN_AIN1_PIN                                       (DL_GPIO_PIN_15)
#define MOTOR_IN_AIN1_IOMUX                                      (IOMUX_PINCM32)
/* Defines for AIN2: GPIOB.16 with pinCMx 33 on package pin 4 */
#define MOTOR_IN_AIN2_PIN                                       (DL_GPIO_PIN_16)
#define MOTOR_IN_AIN2_IOMUX                                      (IOMUX_PINCM33)
/* Defines for BIN1: GPIOB.2 with pinCMx 15 on package pin 50 */
#define MOTOR_IN_BIN1_PIN                                        (DL_GPIO_PIN_2)
#define MOTOR_IN_BIN1_IOMUX                                      (IOMUX_PINCM15)
/* Defines for BIN2: GPIOB.3 with pinCMx 16 on package pin 51 */
#define MOTOR_IN_BIN2_PIN                                        (DL_GPIO_PIN_3)
#define MOTOR_IN_BIN2_IOMUX                                      (IOMUX_PINCM16)
/* Defines for Encoder1B: GPIOB.23 with pinCMx 51 on package pin 22 */
#define GPIO_Encoder_Encoder1B_PORT                                      (GPIOB)
#define GPIO_Encoder_Encoder1B_PIN                              (DL_GPIO_PIN_23)
#define GPIO_Encoder_Encoder1B_IOMUX                             (IOMUX_PINCM51)
/* Defines for Encoder2B: GPIOA.12 with pinCMx 34 on package pin 5 */
#define GPIO_Encoder_Encoder2B_PORT                                      (GPIOA)
#define GPIO_Encoder_Encoder2B_PIN                              (DL_GPIO_PIN_12)
#define GPIO_Encoder_Encoder2B_IOMUX                             (IOMUX_PINCM34)
/* Defines for AD0: GPIOA.13 with pinCMx 35 on package pin 6 */
#define GPIO_TRACE_AD0_PORT                                              (GPIOA)
#define GPIO_TRACE_AD0_PIN                                      (DL_GPIO_PIN_13)
#define GPIO_TRACE_AD0_IOMUX                                     (IOMUX_PINCM35)
/* Defines for AD1: GPIOB.26 with pinCMx 57 on package pin 28 */
#define GPIO_TRACE_AD1_PORT                                              (GPIOB)
#define GPIO_TRACE_AD1_PIN                                      (DL_GPIO_PIN_26)
#define GPIO_TRACE_AD1_IOMUX                                     (IOMUX_PINCM57)
/* Defines for AD2: GPIOA.22 with pinCMx 47 on package pin 18 */
#define GPIO_TRACE_AD2_PORT                                              (GPIOA)
#define GPIO_TRACE_AD2_PIN                                      (DL_GPIO_PIN_22)
#define GPIO_TRACE_AD2_IOMUX                                     (IOMUX_PINCM47)
/* Defines for KEY0: GPIOA.17 with pinCMx 39 on package pin 10 */
#define KEY_KEY0_PORT                                                    (GPIOA)
#define KEY_KEY0_PIN                                            (DL_GPIO_PIN_17)
#define KEY_KEY0_IOMUX                                           (IOMUX_PINCM39)
/* Defines for KEY1: GPIOB.18 with pinCMx 44 on package pin 15 */
#define KEY_KEY1_PORT                                                    (GPIOB)
#define KEY_KEY1_PIN                                            (DL_GPIO_PIN_18)
#define KEY_KEY1_IOMUX                                           (IOMUX_PINCM44)
/* Defines for KEY2: GPIOB.22 with pinCMx 50 on package pin 21 */
#define KEY_KEY2_PORT                                                    (GPIOB)
#define KEY_KEY2_PIN                                            (DL_GPIO_PIN_22)
#define KEY_KEY2_IOMUX                                           (IOMUX_PINCM50)
/* Defines for L0: GPIOB.14 with pinCMx 31 on package pin 2 */
#define LED_L0_PORT                                                      (GPIOB)
#define LED_L0_PIN                                              (DL_GPIO_PIN_14)
#define LED_L0_IOMUX                                             (IOMUX_PINCM31)
/* Defines for L1: GPIOA.7 with pinCMx 14 on package pin 49 */
#define LED_L1_PORT                                                      (GPIOA)
#define LED_L1_PIN                                               (DL_GPIO_PIN_7)
#define LED_L1_IOMUX                                             (IOMUX_PINCM14)
/* Defines for L2: GPIOB.1 with pinCMx 13 on package pin 48 */
#define LED_L2_PORT                                                      (GPIOB)
#define LED_L2_PIN                                               (DL_GPIO_PIN_1)
#define LED_L2_IOMUX                                             (IOMUX_PINCM13)
/* Port definition for Pin Group GPIO_OLED */
#define GPIO_OLED_PORT                                                   (GPIOA)

/* Defines for SCL: GPIOA.1 with pinCMx 2 on package pin 34 */
#define GPIO_OLED_SCL_PIN                                        (DL_GPIO_PIN_1)
#define GPIO_OLED_SCL_IOMUX                                       (IOMUX_PINCM2)
/* Defines for SDA: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_OLED_SDA_PIN                                        (DL_GPIO_PIN_0)
#define GPIO_OLED_SDA_IOMUX                                       (IOMUX_PINCM1)
/* Defines for PIN_1EN: GPIOB.25 with pinCMx 56 on package pin 27 */
#define GPIO_BUJIN_PIN_1EN_PORT                                          (GPIOB)
#define GPIO_BUJIN_PIN_1EN_PIN                                  (DL_GPIO_PIN_25)
#define GPIO_BUJIN_PIN_1EN_IOMUX                                 (IOMUX_PINCM56)
/* Defines for PIN_1DIR: GPIOB.21 with pinCMx 49 on package pin 20 */
#define GPIO_BUJIN_PIN_1DIR_PORT                                         (GPIOB)
#define GPIO_BUJIN_PIN_1DIR_PIN                                 (DL_GPIO_PIN_21)
#define GPIO_BUJIN_PIN_1DIR_IOMUX                                (IOMUX_PINCM49)
/* Defines for PIN_2EN: GPIOA.14 with pinCMx 36 on package pin 7 */
#define GPIO_BUJIN_PIN_2EN_PORT                                          (GPIOA)
#define GPIO_BUJIN_PIN_2EN_PIN                                  (DL_GPIO_PIN_14)
#define GPIO_BUJIN_PIN_2EN_IOMUX                                 (IOMUX_PINCM36)
/* Defines for PIN_2DIR: GPIOA.16 with pinCMx 38 on package pin 9 */
#define GPIO_BUJIN_PIN_2DIR_PORT                                         (GPIOA)
#define GPIO_BUJIN_PIN_2DIR_PIN                                 (DL_GPIO_PIN_16)
#define GPIO_BUJIN_PIN_2DIR_IOMUX                                (IOMUX_PINCM38)
/* Port definition for Pin Group SR04 */
#define SR04_PORT                                                        (GPIOA)

/* Defines for Trig: GPIOA.18 with pinCMx 40 on package pin 11 */
#define SR04_Trig_PIN                                           (DL_GPIO_PIN_18)
#define SR04_Trig_IOMUX                                          (IOMUX_PINCM40)
/* Defines for Echo: GPIOA.15 with pinCMx 37 on package pin 8 */
// pins affected by this interrupt request:["Echo"]
#define SR04_INT_IRQN                                           (GPIOA_INT_IRQn)
#define SR04_INT_IIDX                           (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define SR04_Echo_IIDX                                      (DL_GPIO_IIDX_DIO15)
#define SR04_Echo_PIN                                           (DL_GPIO_PIN_15)
#define SR04_Echo_IOMUX                                          (IOMUX_PINCM37)
/* Port definition for Pin Group GPIO_ADD */
#define GPIO_ADD_PORT                                                    (GPIOB)

/* Defines for PIN_BEEP: GPIOB.17 with pinCMx 43 on package pin 14 */
#define GPIO_ADD_PIN_BEEP_PIN                                   (DL_GPIO_PIN_17)
#define GPIO_ADD_PIN_BEEP_IOMUX                                  (IOMUX_PINCM43)
/* Defines for PIN_LASERPEN: GPIOB.0 with pinCMx 12 on package pin 47 */
#define GPIO_ADD_PIN_LASERPEN_PIN                                (DL_GPIO_PIN_0)
#define GPIO_ADD_PIN_LASERPEN_IOMUX                              (IOMUX_PINCM12)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_Motor_init(void);
void SYSCFG_DL_PWM_BUJIN_X_init(void);
void SYSCFG_DL_PWM_BUJIN_Y_init(void);
void SYSCFG_DL_Encoder1A_init(void);
void SYSCFG_DL_Encoder2A_init(void);
void SYSCFG_DL_motor_CLOCK_init(void);
void SYSCFG_DL_I2C_1_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_UART_1_init(void);
void SYSCFG_DL_UART_3_init(void);
void SYSCFG_DL_ADC_CHASE_init(void);
void SYSCFG_DL_DMA_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
