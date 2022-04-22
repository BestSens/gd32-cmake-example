/*!
    \file    startup_gd32f450_470.s
    \brief   start up file

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
    \version 2022-03-09, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

// <h> Stack Configuration
//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>

.equ Stack_Size, 0x00000400

				.section STACK, "aw"
				.balign 8
Stack_Mem:
.org Stack_Mem + Stack_Size
__initial_sp:


// <h> Heap Configuration
//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>

.equ Heap_Size, 0x00000400

				.section HEAP, "aw"
				.balign 8
__heap_base:
Heap_Mem:
.org Heap_Mem + Heap_Size
__heap_limit:

                .eabi_attribute Tag_ABI_align_preserved, 1
                .thumb

                /* reset Vector Mapped to at Address 0 */
				.section RESET, "a"
                .global  __Vectors
                .global  __Vectors_End
                .global  __Vectors_Size

__Vectors:      .word     __initial_sp                      // Top of Stack
                .word     Reset_Handler                     // Reset Handler
                .word     NMI_Handler                       // NMI Handler
                .word     HardFault_Handler                 // Hard Fault Handler
                .word     MemManage_Handler                 // MPU Fault Handler
                .word     BusFault_Handler                  // Bus Fault Handler
                .word     UsageFault_Handler                // Usage Fault Handler
                .word     0                                 // Reserved
                .word     0                                 // Reserved
                .word     0                                 // Reserved
                .word     0                                 // Reserved
                .word     SVC_Handler                       // SVCall Handler
                .word     DebugMon_Handler                  // Debug Monitor Handler
                .word     0                                 // Reserved
                .word     PendSV_Handler                    // PendSV Handler
                .word     SysTick_Handler                   // SysTick Handler

                /* external interrupts handler */
                .word     WWDGT_IRQHandler                  // 16:Window Watchdog Timer
                .word     LVD_IRQHandler                    // 17:LVD through EXTI Line detect
                .word     TAMPER_STAMP_IRQHandler           // 18:Tamper and TimeStamp through EXTI Line detect
                .word     RTC_WKUP_IRQHandler               // 19:RTC Wakeup through EXTI Line
                .word     FMC_IRQHandler                    // 20:FMC
                .word     RCU_CTC_IRQHandler                // 21:RCU and CTC
                .word     EXTI0_IRQHandler                  // 22:EXTI Line 0
                .word     EXTI1_IRQHandler                  // 23:EXTI Line 1
                .word     EXTI2_IRQHandler                  // 24:EXTI Line 2
                .word     EXTI3_IRQHandler                  // 25:EXTI Line 3
                .word     EXTI4_IRQHandler                  // 26:EXTI Line 4
                .word     DMA0_Channel0_IRQHandler          // 27:DMA0 Channel0
                .word     DMA0_Channel1_IRQHandler          // 28:DMA0 Channel1
                .word     DMA0_Channel2_IRQHandler          // 29:DMA0 Channel2
                .word     DMA0_Channel3_IRQHandler          // 30:DMA0 Channel3
                .word     DMA0_Channel4_IRQHandler          // 31:DMA0 Channel4
                .word     DMA0_Channel5_IRQHandler          // 32:DMA0 Channel5
                .word     DMA0_Channel6_IRQHandler          // 33:DMA0 Channel6
                .word     ADC_IRQHandler                    // 34:ADC
                .word     CAN0_TX_IRQHandler                // 35:CAN0 TX
                .word     CAN0_RX0_IRQHandler               // 36:CAN0 RX0
                .word     CAN0_RX1_IRQHandler               // 37:CAN0 RX1
                .word     CAN0_EWMC_IRQHandler              // 38:CAN0 EWMC
                .word     EXTI5_9_IRQHandler                // 39:EXTI5 to EXTI9
                .word     TIMER0_BRK_TIMER8_IRQHandler      // 40:TIMER0 Break and TIMER8
                .word     TIMER0_UP_TIMER9_IRQHandler       // 41:TIMER0 Update and TIMER9
                .word     TIMER0_TRG_CMT_TIMER10_IRQHandler // 42:TIMER0 Trigger and Commutation and TIMER10
                .word     TIMER0_Channel_IRQHandler         // 43:TIMER0 Capture Compare
                .word     TIMER1_IRQHandler                 // 44:TIMER1
                .word     TIMER2_IRQHandler                 // 45:TIMER2
                .word     TIMER3_IRQHandler                 // 46:TIMER3
                .word     I2C0_EV_IRQHandler                // 47:I2C0 Event
                .word     I2C0_ER_IRQHandler                // 48:I2C0 Error
                .word     I2C1_EV_IRQHandler                // 49:I2C1 Event
                .word     I2C1_ER_IRQHandler                // 50:I2C1 Error
                .word     SPI0_IRQHandler                   // 51:SPI0
                .word     SPI1_IRQHandler                   // 52:SPI1
                .word     USART0_IRQHandler                 // 53:USART0
                .word     USART1_IRQHandler                 // 54:USART1
                .word     USART2_IRQHandler                 // 55:USART2
                .word     EXTI10_15_IRQHandler              // 56:EXTI10 to EXTI15
                .word     RTC_Alarm_IRQHandler              // 57:RTC Alarm
                .word     USBFS_WKUP_IRQHandler             // 58:USBFS Wakeup
                .word     TIMER7_BRK_TIMER11_IRQHandler     // 59:TIMER7 Break and TIMER11
                .word     TIMER7_UP_TIMER12_IRQHandler      // 60:TIMER7 Update and TIMER12
                .word     TIMER7_TRG_CMT_TIMER13_IRQHandler // 61:TIMER7 Trigger and Commutation and TIMER13
                .word     TIMER7_Channel_IRQHandler         // 62:TIMER7 Channel Capture Compare
                .word     DMA0_Channel7_IRQHandler          // 63:DMA0 Channel7
                .word     EXMC_IRQHandler                   // 64:EXMC
                .word     SDIO_IRQHandler                   // 65:SDIO
                .word     TIMER4_IRQHandler                 // 66:TIMER4
                .word     SPI2_IRQHandler                   // 67:SPI2
                .word     UART3_IRQHandler                  // 68:UART3
                .word     UART4_IRQHandler                  // 69:UART4
                .word     TIMER5_DAC_IRQHandler             // 70:TIMER5 and DAC0 DAC1 Underrun error
                .word     TIMER6_IRQHandler                 // 71:TIMER6
                .word     DMA1_Channel0_IRQHandler          // 72:DMA1 Channel0
                .word     DMA1_Channel1_IRQHandler          // 73:DMA1 Channel1
                .word     DMA1_Channel2_IRQHandler          // 74:DMA1 Channel2
                .word     DMA1_Channel3_IRQHandler          // 75:DMA1 Channel3
                .word     DMA1_Channel4_IRQHandler          // 76:DMA1 Channel4
                .word     ENET_IRQHandler                   // 77:Ethernet
                .word     ENET_WKUP_IRQHandler              // 78:Ethernet Wakeup through EXTI Line
                .word     CAN1_TX_IRQHandler                // 79:CAN1 TX
                .word     CAN1_RX0_IRQHandler               // 80:CAN1 RX0
                .word     CAN1_RX1_IRQHandler               // 81:CAN1 RX1
                .word     CAN1_EWMC_IRQHandler              // 82:CAN1 EWMC
                .word     USBFS_IRQHandler                  // 83:USBFS
                .word     DMA1_Channel5_IRQHandler          // 84:DMA1 Channel5
                .word     DMA1_Channel6_IRQHandler          // 85:DMA1 Channel6
                .word     DMA1_Channel7_IRQHandler          // 86:DMA1 Channel7
                .word     USART5_IRQHandler                 // 87:USART5
                .word     I2C2_EV_IRQHandler                // 88:I2C2 Event
                .word     I2C2_ER_IRQHandler                // 89:I2C2 Error
                .word     USBHS_EP1_Out_IRQHandler          // 90:USBHS Endpoint 1 Out
                .word     USBHS_EP1_In_IRQHandler           // 91:USBHS Endpoint 1 in
                .word     USBHS_WKUP_IRQHandler             // 92:USBHS Wakeup through EXTI Line
                .word     USBHS_IRQHandler                  // 93:USBHS
                .word     DCI_IRQHandler                    // 94:DCI
                .word     0                                 // 95:Reserved
                .word     TRNG_IRQHandler                   // 96:TRNG
                .word     FPU_IRQHandler                    // 97:FPU
                .word     UART6_IRQHandler                  // 98:UART6
                .word     UART7_IRQHandler                  // 99:UART7
                .word     SPI3_IRQHandler                   // 100:SPI3
                .word     SPI4_IRQHandler                   // 101:SPI4
                .word     SPI5_IRQHandler                   // 102:SPI5
                .word     0                                 // 103:Reserved
                .word     TLI_IRQHandler                    // 104:TLI
                .word     TLI_ER_IRQHandler                 // 105:TLI Error
                .word     IPA_IRQHandler                    // 106:IPA

__Vectors_End:

.equ __Vectors_Size, __Vectors_End - __Vectors

				.section .text,"ax"

/* reset Handler */
Reset_Handler:
                .weak  Reset_Handler
                .global  SystemInit
                .global  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0

/* dummy Exception Handlers */
NMI_Handler:
                .weak  NMI_Handler
                B       .
HardFault_Handler:
                .weak  HardFault_Handler
                B       .
MemManage_Handler:
                .weak  MemManage_Handler
                B       .
BusFault_Handler:
                .weak  BusFault_Handler
                B       .
UsageFault_Handler:
                .weak  UsageFault_Handler
                B       .
SVC_Handler:
                .weak  SVC_Handler
                B       .
DebugMon_Handler:
                .weak  DebugMon_Handler
                B       .
PendSV_Handler:
                .weak  PendSV_Handler
                B       .
SysTick_Handler:
                .weak  SysTick_Handler
                B       .

Default_Handler:
                /* external interrupts handler */
                .weak  WWDGT_IRQHandler
                .weak  LVD_IRQHandler
                .weak  TAMPER_STAMP_IRQHandler
                .weak  RTC_WKUP_IRQHandler
                .weak  FMC_IRQHandler
                .weak  RCU_CTC_IRQHandler
                .weak  EXTI0_IRQHandler
                .weak  EXTI1_IRQHandler
                .weak  EXTI2_IRQHandler
                .weak  EXTI3_IRQHandler
                .weak  EXTI4_IRQHandler
                .weak  DMA0_Channel0_IRQHandler
                .weak  DMA0_Channel1_IRQHandler
                .weak  DMA0_Channel2_IRQHandler
                .weak  DMA0_Channel3_IRQHandler
                .weak  DMA0_Channel4_IRQHandler
                .weak  DMA0_Channel5_IRQHandler
                .weak  DMA0_Channel6_IRQHandler
                .weak  ADC_IRQHandler
                .weak  CAN0_TX_IRQHandler
                .weak  CAN0_RX0_IRQHandler
                .weak  CAN0_RX1_IRQHandler
                .weak  CAN0_EWMC_IRQHandler
                .weak  EXTI5_9_IRQHandler
                .weak  TIMER0_BRK_TIMER8_IRQHandler
                .weak  TIMER0_UP_TIMER9_IRQHandler
                .weak  TIMER0_TRG_CMT_TIMER10_IRQHandler
                .weak  TIMER0_Channel_IRQHandler
                .weak  TIMER1_IRQHandler
                .weak  TIMER2_IRQHandler
                .weak  TIMER3_IRQHandler
                .weak  I2C0_EV_IRQHandler
                .weak  I2C0_ER_IRQHandler
                .weak  I2C1_EV_IRQHandler
                .weak  I2C1_ER_IRQHandler
                .weak  SPI0_IRQHandler
                .weak  SPI1_IRQHandler
                .weak  USART0_IRQHandler
                .weak  USART1_IRQHandler
                .weak  USART2_IRQHandler
                .weak  EXTI10_15_IRQHandler
                .weak  RTC_Alarm_IRQHandler
                .weak  USBFS_WKUP_IRQHandler
                .weak  TIMER7_BRK_TIMER11_IRQHandler
                .weak  TIMER7_UP_TIMER12_IRQHandler
                .weak  TIMER7_TRG_CMT_TIMER13_IRQHandler
                .weak  TIMER7_Channel_IRQHandler
                .weak  DMA0_Channel7_IRQHandler
                .weak  EXMC_IRQHandler
                .weak  SDIO_IRQHandler
                .weak  TIMER4_IRQHandler
                .weak  SPI2_IRQHandler
                .weak  UART3_IRQHandler
                .weak  UART4_IRQHandler
                .weak  TIMER5_DAC_IRQHandler
                .weak  TIMER6_IRQHandler
                .weak  DMA1_Channel0_IRQHandler
                .weak  DMA1_Channel1_IRQHandler
                .weak  DMA1_Channel2_IRQHandler
                .weak  DMA1_Channel3_IRQHandler
                .weak  DMA1_Channel4_IRQHandler
                .weak  ENET_IRQHandler
                .weak  ENET_WKUP_IRQHandler
                .weak  CAN1_TX_IRQHandler
                .weak  CAN1_RX0_IRQHandler
                .weak  CAN1_RX1_IRQHandler
                .weak  CAN1_EWMC_IRQHandler
                .weak  USBFS_IRQHandler
                .weak  DMA1_Channel5_IRQHandler
                .weak  DMA1_Channel6_IRQHandler
                .weak  DMA1_Channel7_IRQHandler
                .weak  USART5_IRQHandler
                .weak  I2C2_EV_IRQHandler
                .weak  I2C2_ER_IRQHandler
                .weak  USBHS_EP1_Out_IRQHandler
                .weak  USBHS_EP1_In_IRQHandler
                .weak  USBHS_WKUP_IRQHandler
                .weak  USBHS_IRQHandler
                .weak  DCI_IRQHandler
                .weak  TRNG_IRQHandler
                .weak  FPU_IRQHandler
                .weak  UART6_IRQHandler
                .weak  UART7_IRQHandler
                .weak  SPI3_IRQHandler
                .weak  SPI4_IRQHandler
                .weak  SPI5_IRQHandler
                .weak  TLI_IRQHandler
                .weak  TLI_ER_IRQHandler
                .weak  IPA_IRQHandler

/* external interrupts handler */
WWDGT_IRQHandler:
LVD_IRQHandler:
TAMPER_STAMP_IRQHandler:
RTC_WKUP_IRQHandler:
FMC_IRQHandler:
RCU_CTC_IRQHandler:
EXTI0_IRQHandler:
EXTI1_IRQHandler:
EXTI2_IRQHandler:
EXTI3_IRQHandler:
EXTI4_IRQHandler:
DMA0_Channel0_IRQHandler:
DMA0_Channel1_IRQHandler:
DMA0_Channel2_IRQHandler:
DMA0_Channel3_IRQHandler:
DMA0_Channel4_IRQHandler:
DMA0_Channel5_IRQHandler:
DMA0_Channel6_IRQHandler:
ADC_IRQHandler:
CAN0_TX_IRQHandler:
CAN0_RX0_IRQHandler:
CAN0_RX1_IRQHandler:
CAN0_EWMC_IRQHandler:
EXTI5_9_IRQHandler:
TIMER0_BRK_TIMER8_IRQHandler:
TIMER0_UP_TIMER9_IRQHandler:
TIMER0_TRG_CMT_TIMER10_IRQHandler:
TIMER0_Channel_IRQHandler:
TIMER1_IRQHandler:
TIMER2_IRQHandler:
TIMER3_IRQHandler:
I2C0_EV_IRQHandler:
I2C0_ER_IRQHandler:
I2C1_EV_IRQHandler:
I2C1_ER_IRQHandler:
SPI0_IRQHandler:
SPI1_IRQHandler:
USART0_IRQHandler:
USART1_IRQHandler:
USART2_IRQHandler:
EXTI10_15_IRQHandler:
RTC_Alarm_IRQHandler:
USBFS_WKUP_IRQHandler:
TIMER7_BRK_TIMER11_IRQHandler:
TIMER7_UP_TIMER12_IRQHandler:
TIMER7_TRG_CMT_TIMER13_IRQHandler:
TIMER7_Channel_IRQHandler:
DMA0_Channel7_IRQHandler:
EXMC_IRQHandler:
SDIO_IRQHandler:
TIMER4_IRQHandler:
SPI2_IRQHandler:
UART3_IRQHandler:
UART4_IRQHandler:
TIMER5_DAC_IRQHandler:
TIMER6_IRQHandler:
DMA1_Channel0_IRQHandler:
DMA1_Channel1_IRQHandler:
DMA1_Channel2_IRQHandler:
DMA1_Channel3_IRQHandler:
DMA1_Channel4_IRQHandler:
ENET_IRQHandler:
ENET_WKUP_IRQHandler:
CAN1_TX_IRQHandler:
CAN1_RX0_IRQHandler:
CAN1_RX1_IRQHandler:
CAN1_EWMC_IRQHandler:
USBFS_IRQHandler:
DMA1_Channel5_IRQHandler:
DMA1_Channel6_IRQHandler:
DMA1_Channel7_IRQHandler:
USART5_IRQHandler:
I2C2_EV_IRQHandler:
I2C2_ER_IRQHandler:
USBHS_EP1_Out_IRQHandler:
USBHS_EP1_In_IRQHandler:
USBHS_WKUP_IRQHandler:
USBHS_IRQHandler:
DCI_IRQHandler:
TRNG_IRQHandler:
FPU_IRQHandler:
UART6_IRQHandler:
UART7_IRQHandler:
SPI3_IRQHandler:
SPI4_IRQHandler:
SPI5_IRQHandler:
TLI_IRQHandler:
TLI_ER_IRQHandler:
IPA_IRQHandler:

                B       .

                .balign

// user Initial Stack & Heap

                .ifdef __MICROLIB

                .global  __initial_sp
                .global  __heap_base
                .global  __heap_limit

                .else

                .global  __use_two_region_memory
                .global  __user_initial_stackheap

__user_initial_stackheap:
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                .balign

                .endif

                .end
