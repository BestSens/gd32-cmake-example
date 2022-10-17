/**
  ******************************************************************************
  * @file      startup_stm32f446xx.s
  * @author    MCD Application Team
  * @brief     STM32F446xx Devices vector table for GCC based toolchains. 
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M4 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
    
  .syntax unified
  .cpu cortex-m4
  .fpu softvfp
  .thumb

.global  g_pfnVectors
.global  Default_Handler

/* start address for the initialization values of the .data section. 
defined in linker script */
.word  _sidata
/* start address for the .data section. defined in linker script */  
.word  _sdata
/* end address for the .data section. defined in linker script */
.word  _edata
/* start address for the .bss section. defined in linker script */
.word  _sbss
/* end address for the .bss section. defined in linker script */
.word  _ebss
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called. 
 * @param  None
 * @retval : None
*/

    .section  .text.Reset_Handler
  .weak  Reset_Handler
  .type  Reset_Handler, %function
Reset_Handler:  
  ldr   sp, =_estack      /* set stack pointer */

/* Copy the data segment initializers from flash to SRAM */  
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit
  
/* Zero fill the bss segment. */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

/* Call the clock system initialization function.*/
  bl  SystemInit   
/* Call static constructors */
    bl __libc_init_array
/* Call the application's entry point.*/
  bl  main
  bx  lr    
.size  Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an 
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None     
 * @retval None       
*/
    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
* 
*******************************************************************************/
   .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors
   
   
g_pfnVectors:
  .word  _estack
  .word  Reset_Handler

  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  
  /* External Interrupts */
  .word     WWDG_IRQHandler                   /* 16:Window WatchDog              */                                        
  .word     LVD_IRQHandler                    /* 17:LVD through EXTI Line detection */                        
  .word     TAMP_STAMP_IRQHandler             /* 18:Tamper and TimeStamps through the EXTI line */            
  .word     RTC_WKUP_IRQHandler               /* 19:RTC Wakeup through the EXTI line */                      
  .word     FLASH_IRQHandler                  /* 20:FLASH                        */                                          
  .word     RCC_IRQHandler                    /* 21:RCC                          */                                            
  .word     EXTI0_IRQHandler                  /* 22:EXTI Line0                   */                        
  .word     EXTI1_IRQHandler                  /* 23:EXTI Line1                   */                          
  .word     EXTI2_IRQHandler                  /* 24:EXTI Line2                   */                          
  .word     EXTI3_IRQHandler                  /* 25:EXTI Line3                   */                          
  .word     EXTI4_IRQHandler                  /* 26:EXTI Line4                   */                          
  .word     DMA0_Channel0_IRQHandler          /* 27:DMA0 Channel0                */                  
  .word     DMA0_Channel1_IRQHandler          /* 28:DMA0 Channel1                */                   
  .word     DMA0_Channel2_IRQHandler          /* 29:DMA0 Channel2                */                   
  .word     DMA0_Channel3_IRQHandler          /* 30:DMA0 Channel3                */                   
  .word     DMA0_Channel4_IRQHandler          /* 31:DMA0 Channel4                */                   
  .word     DMA0_Channel5_IRQHandler          /* 32:DMA0 Channel5                */                   
  .word     DMA0_Channel6_IRQHandler          /* 33:DMA0 Channel6                */                   
  .word     ADC_IRQHandler                    /* 34:ADC1, ADC2 and ADC3s         */                   
  .word     CAN1_TX_IRQHandler                /* 35:CAN1 TX                      */                         
  .word     CAN1_RX0_IRQHandler               /* 36:CAN1 RX0                     */                          
  .word     CAN1_RX1_IRQHandler               /* 37:CAN1 RX1                     */                          
  .word     CAN1_SCE_IRQHandler               /* 38:CAN1 SCE                     */                          
  .word     EXTI9_5_IRQHandler                /* 39:External Line[9:5]s          */                          
  .word     TIM1_BRK_TIM9_IRQHandler          /* 40:TIM1 Break and TIM9          */         
  .word     TIM1_UP_TIM10_IRQHandler          /* 41:TIM1 Update and TIM10        */         
  .word     TIM1_TRG_COM_TIM11_IRQHandler     /* 42:TIM1 Trigger and Commutation and TIM11 */
  .word     TIM1_CC_IRQHandler                /* 43:TIM1 Capture Compare         */                          
  .word     TIM2_IRQHandler                   /* 44:TIM2                         */                   
  .word     TIM3_IRQHandler                   /* 45:TIM3                         */                   
  .word     TIM4_IRQHandler                   /* 46:TIM4                         */                   
  .word     I2C1_EV_IRQHandler                /* 47:I2C1 Event                   */                          
  .word     I2C1_ER_IRQHandler                /* 48:I2C1 Error                   */                          
  .word     I2C2_EV_IRQHandler                /* 49:I2C2 Event                   */                          
  .word     I2C2_ER_IRQHandler                /* 50:I2C2 Error                   */                            
  .word     SPI0_IRQHandler                   /* 51:SPI0                         */                   
  .word     SPI1_IRQHandler                   /* 52:SPI1                         */                   
  .word     USART1_IRQHandler                 /* 53:USART1                       */                   
  .word     USART2_IRQHandler                 /* 54:USART2                       */                   
  .word     USART3_IRQHandler                 /* 55:USART3                       */                   
  .word     EXTI15_10_IRQHandler              /* 56:External Line[15:10]s        */                          
  .word     RTC_Alarm_IRQHandler              /* 57:RTC Alarm (A and B) through EXTI Line */                 
  .word     OTG_FS_WKUP_IRQHandler            /* 58:USB OTG FS Wakeup through EXTI line */                       
  .word     TIM8_BRK_TIM12_IRQHandler         /* 59:TIM8 Break and TIM12         */         
  .word     TIM8_UP_TIM13_IRQHandler          /* 60:TIM8 Update and TIM13        */         
  .word     TIM8_TRG_COM_TIM14_IRQHandler     /* 61:TIM8 Trigger and Commutation and TIM14 */
  .word     TIM8_CC_IRQHandler                /* 62:TIM8 Capture Compare         */                          
  .word     DMA0_Channel7_IRQHandler          /* 63:DMA0 Channel7                */                          
  .word     FMC_IRQHandler                    /* 64:FMC                          */                   
  .word     SDIO_IRQHandler                   /* 65:SDIO                         */                   
  .word     TIM5_IRQHandler                   /* 66:TIM5                         */                   
  .word     SPI2_IRQHandler                   /* 67:SPI2                         */                   
  .word     UART4_IRQHandler                  /* 68:UART4                        */                   
  .word     UART5_IRQHandler                  /* 69:UART5                        */                   
  .word     TIM6_DAC_IRQHandler               /* 70:TIM6 and DAC1&2 underrun errors */                   
  .word     TIM7_IRQHandler                   /* 71:TIM7                         */
  .word     DMA1_Channel0_IRQHandler          /* 72:DMA1 Channel0                */                   
  .word     DMA1_Channel1_IRQHandler          /* 73:DMA1 Channel1                */                   
  .word     DMA1_Channel2_IRQHandler          /* 74:DMA1 Channel2                */                   
  .word     DMA1_Channel3_IRQHandler          /* 75:DMA1 Channel3                */                   
  .word     DMA1_Channel4_IRQHandler          /* 76:DMA1 Channel4                */                   
  .word     ENET_IRQHandler                   /* 77:Ethernet                     */                   
  .word     ENET_WKUP_IRQHandler              /* 78:Ethernet Wakeup through EXTI Line                     */                     
  .word     CAN2_TX_IRQHandler                /* 79:CAN2 TX                      */                          
  .word     CAN2_RX0_IRQHandler               /* 80:CAN2 RX0                     */                          
  .word     CAN2_RX1_IRQHandler               /* 81:CAN2 RX1                     */                          
  .word     CAN2_SCE_IRQHandler               /* 82:CAN2 SCE                     */                          
  .word     OTG_FS_IRQHandler                 /* 83:USB OTG FS                   */                   
  .word     DMA1_Channel5_IRQHandler          /* 84:DMA1 Channel5                */                   
  .word     DMA1_Channel6_IRQHandler          /* 85:DMA1 Channel6                */                   
  .word     DMA1_Channel7_IRQHandler          /* 86:DMA1 Channel7                */                   
  .word     USART6_IRQHandler                 /* 87:USART6                       */                    
  .word     I2C3_EV_IRQHandler                /* 88:I2C3 event                   */                          
  .word     I2C3_ER_IRQHandler                /* 89:I2C3 error                   */                          
  .word     OTG_HS_EP1_OUT_IRQHandler         /* 90:USB OTG HS End Point 1 Out   */                   
  .word     OTG_HS_EP1_IN_IRQHandler          /* 91:USB OTG HS End Point 1 In    */                   
  .word     OTG_HS_WKUP_IRQHandler            /* 92:USB OTG HS Wakeup through EXTI */                         
  .word     OTG_HS_IRQHandler                 /* 93:USB OTG HS                   */                   
  .word     DCMI_IRQHandler                   /* 94:DCMI                         */                   
  .word     0                                 /* 95:Reserved                     */                   
  .word     TRNG_IRQHandler                   /* 96:TRNG                         */
  .word     FPU_IRQHandler                    /* 97:FPU                          */
  .word     UART6_IRQHandler                  /* 98:UART6                        */
  .word     UART6_IRQHandler                  /* 99:UART7                        */
  .word     SPI3_IRQHandler                   /* 100:SPI3                        */
  .word     SPI4_IRQHandler                   /* 101:SPI4                        */
  .word     SPI5_IRQHandler                   /* 102:SPI5                        */
  .word     0                                 /* 103:Reserved                    */
  .word     TLI_IRQHandler                    /* 104:TLI                         */
  .word     TLI_ER_IRQHandler                 /* 105:TLI Error                   */
  .word     IPA_IRQHandler                    /* 106:IPA                         */
  
/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler. 
* As they are weak aliases, any function with the same name will override 
* this definition.
* 
*******************************************************************************/
   .weak      NMI_Handler
   .thumb_set NMI_Handler,Default_Handler
  
   .weak      HardFault_Handler
   .thumb_set HardFault_Handler,Default_Handler
  
   .weak      MemManage_Handler
   .thumb_set MemManage_Handler,Default_Handler
  
   .weak      BusFault_Handler
   .thumb_set BusFault_Handler,Default_Handler

   .weak      UsageFault_Handler
   .thumb_set UsageFault_Handler,Default_Handler

   .weak      SVC_Handler
   .thumb_set SVC_Handler,Default_Handler

   .weak      DebugMon_Handler
   .thumb_set DebugMon_Handler,Default_Handler

   .weak      PendSV_Handler
   .thumb_set PendSV_Handler,Default_Handler

   .weak      SysTick_Handler
   .thumb_set SysTick_Handler,Default_Handler              
  
   .weak      WWDG_IRQHandler                   
   .thumb_set WWDG_IRQHandler,Default_Handler      
                  
   .weak      LVD_IRQHandler      
   .thumb_set LVD_IRQHandler,Default_Handler
               
   .weak      TAMP_STAMP_IRQHandler            
   .thumb_set TAMP_STAMP_IRQHandler,Default_Handler
            
   .weak      RTC_WKUP_IRQHandler                  
   .thumb_set RTC_WKUP_IRQHandler,Default_Handler
            
   .weak      FLASH_IRQHandler         
   .thumb_set FLASH_IRQHandler,Default_Handler
                  
   .weak      RCC_IRQHandler      
   .thumb_set RCC_IRQHandler,Default_Handler
                  
   .weak      EXTI0_IRQHandler         
   .thumb_set EXTI0_IRQHandler,Default_Handler
                  
   .weak      EXTI1_IRQHandler         
   .thumb_set EXTI1_IRQHandler,Default_Handler
                     
   .weak      EXTI2_IRQHandler         
   .thumb_set EXTI2_IRQHandler,Default_Handler 
                 
   .weak      EXTI3_IRQHandler         
   .thumb_set EXTI3_IRQHandler,Default_Handler
                        
   .weak      EXTI4_IRQHandler         
   .thumb_set EXTI4_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel0_IRQHandler               
   .thumb_set DMA0_Channel0_IRQHandler,Default_Handler
         
   .weak      DMA0_Channel1_IRQHandler               
   .thumb_set DMA0_Channel1_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel2_IRQHandler               
   .thumb_set DMA0_Channel2_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel3_IRQHandler               
   .thumb_set DMA0_Channel3_IRQHandler,Default_Handler 
                 
   .weak      DMA0_Channel4_IRQHandler              
   .thumb_set DMA0_Channel4_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel5_IRQHandler               
   .thumb_set DMA0_Channel5_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel6_IRQHandler               
   .thumb_set DMA0_Channel6_IRQHandler,Default_Handler
                  
   .weak      ADC_IRQHandler      
   .thumb_set ADC_IRQHandler,Default_Handler
               
   .weak      CAN1_TX_IRQHandler   
   .thumb_set CAN1_TX_IRQHandler,Default_Handler
            
   .weak      CAN1_RX0_IRQHandler                  
   .thumb_set CAN1_RX0_IRQHandler,Default_Handler
                           
   .weak      CAN1_RX1_IRQHandler                  
   .thumb_set CAN1_RX1_IRQHandler,Default_Handler
            
   .weak      CAN1_SCE_IRQHandler                  
   .thumb_set CAN1_SCE_IRQHandler,Default_Handler
            
   .weak      EXTI9_5_IRQHandler   
   .thumb_set EXTI9_5_IRQHandler,Default_Handler
            
   .weak      TIM1_BRK_TIM9_IRQHandler            
   .thumb_set TIM1_BRK_TIM9_IRQHandler,Default_Handler
            
   .weak      TIM1_UP_TIM10_IRQHandler            
   .thumb_set TIM1_UP_TIM10_IRQHandler,Default_Handler

   .weak      TIM1_TRG_COM_TIM11_IRQHandler      
   .thumb_set TIM1_TRG_COM_TIM11_IRQHandler,Default_Handler
      
   .weak      TIM1_CC_IRQHandler   
   .thumb_set TIM1_CC_IRQHandler,Default_Handler
                  
   .weak      TIM2_IRQHandler            
   .thumb_set TIM2_IRQHandler,Default_Handler
                  
   .weak      TIM3_IRQHandler            
   .thumb_set TIM3_IRQHandler,Default_Handler
                  
   .weak      TIM4_IRQHandler            
   .thumb_set TIM4_IRQHandler,Default_Handler
                  
   .weak      I2C1_EV_IRQHandler   
   .thumb_set I2C1_EV_IRQHandler,Default_Handler
                     
   .weak      I2C1_ER_IRQHandler   
   .thumb_set I2C1_ER_IRQHandler,Default_Handler
                     
   .weak      I2C2_EV_IRQHandler   
   .thumb_set I2C2_EV_IRQHandler,Default_Handler
                  
   .weak      I2C2_ER_IRQHandler   
   .thumb_set I2C2_ER_IRQHandler,Default_Handler
                           
   .weak      SPI0_IRQHandler            
   .thumb_set SPI0_IRQHandler,Default_Handler
                        
   .weak      SPI1_IRQHandler            
   .thumb_set SPI1_IRQHandler,Default_Handler
                  
   .weak      USART1_IRQHandler      
   .thumb_set USART1_IRQHandler,Default_Handler
                     
   .weak      USART2_IRQHandler      
   .thumb_set USART2_IRQHandler,Default_Handler
                     
   .weak      USART3_IRQHandler      
   .thumb_set USART3_IRQHandler,Default_Handler
                  
   .weak      EXTI15_10_IRQHandler               
   .thumb_set EXTI15_10_IRQHandler,Default_Handler
               
   .weak      RTC_Alarm_IRQHandler               
   .thumb_set RTC_Alarm_IRQHandler,Default_Handler
            
   .weak      OTG_FS_WKUP_IRQHandler         
   .thumb_set OTG_FS_WKUP_IRQHandler,Default_Handler
            
   .weak      TIM8_BRK_TIM12_IRQHandler         
   .thumb_set TIM8_BRK_TIM12_IRQHandler,Default_Handler
         
   .weak      TIM8_UP_TIM13_IRQHandler            
   .thumb_set TIM8_UP_TIM13_IRQHandler,Default_Handler
         
   .weak      TIM8_TRG_COM_TIM14_IRQHandler      
   .thumb_set TIM8_TRG_COM_TIM14_IRQHandler,Default_Handler
      
   .weak      TIM8_CC_IRQHandler   
   .thumb_set TIM8_CC_IRQHandler,Default_Handler
                  
   .weak      DMA0_Channel7_IRQHandler               
   .thumb_set DMA0_Channel7_IRQHandler,Default_Handler
                     
   .weak      FMC_IRQHandler            
   .thumb_set FMC_IRQHandler,Default_Handler
                     
   .weak      SDIO_IRQHandler            
   .thumb_set SDIO_IRQHandler,Default_Handler
                     
   .weak      TIM5_IRQHandler            
   .thumb_set TIM5_IRQHandler,Default_Handler
                     
   .weak      SPI2_IRQHandler            
   .thumb_set SPI2_IRQHandler,Default_Handler
                     
   .weak      UART4_IRQHandler         
   .thumb_set UART4_IRQHandler,Default_Handler
                  
   .weak      UART5_IRQHandler         
   .thumb_set UART5_IRQHandler,Default_Handler
                  
   .weak      TIM6_DAC_IRQHandler                  
   .thumb_set TIM6_DAC_IRQHandler,Default_Handler
               
   .weak      TIM7_IRQHandler            
   .thumb_set TIM7_IRQHandler,Default_Handler
         
   .weak      DMA1_Channel0_IRQHandler               
   .thumb_set DMA1_Channel0_IRQHandler,Default_Handler
               
   .weak      DMA1_Channel1_IRQHandler               
   .thumb_set DMA1_Channel1_IRQHandler,Default_Handler
                  
   .weak      DMA1_Channel2_IRQHandler               
   .thumb_set DMA1_Channel2_IRQHandler,Default_Handler
            
   .weak      DMA1_Channel3_IRQHandler               
   .thumb_set DMA1_Channel3_IRQHandler,Default_Handler
            
   .weak      DMA1_Channel4_IRQHandler               
   .thumb_set DMA1_Channel4_IRQHandler,Default_Handler

   .weak      ENET_IRQHandler
   .thumb_set ENET_IRQHandler,Default_Handler

   .weak      ENET_WKUP_IRQHandler
   .thumb_set ENET_WKUP_IRQHandler,Default_Handler

   .weak      CAN2_TX_IRQHandler   
   .thumb_set CAN2_TX_IRQHandler,Default_Handler
                           
   .weak      CAN2_RX0_IRQHandler                  
   .thumb_set CAN2_RX0_IRQHandler,Default_Handler
                           
   .weak      CAN2_RX1_IRQHandler                  
   .thumb_set CAN2_RX1_IRQHandler,Default_Handler
                           
   .weak      CAN2_SCE_IRQHandler                  
   .thumb_set CAN2_SCE_IRQHandler,Default_Handler
                           
   .weak      OTG_FS_IRQHandler      
   .thumb_set OTG_FS_IRQHandler,Default_Handler
                     
   .weak      DMA1_Channel5_IRQHandler               
   .thumb_set DMA1_Channel5_IRQHandler,Default_Handler
                  
   .weak      DMA1_Channel6_IRQHandler               
   .thumb_set DMA1_Channel6_IRQHandler,Default_Handler
                  
   .weak      DMA1_Channel7_IRQHandler               
   .thumb_set DMA1_Channel7_IRQHandler,Default_Handler
                  
   .weak      USART6_IRQHandler      
   .thumb_set USART6_IRQHandler,Default_Handler
                        
   .weak      I2C3_EV_IRQHandler   
   .thumb_set I2C3_EV_IRQHandler,Default_Handler
                        
   .weak      I2C3_ER_IRQHandler   
   .thumb_set I2C3_ER_IRQHandler,Default_Handler
                        
   .weak      OTG_HS_EP1_OUT_IRQHandler         
   .thumb_set OTG_HS_EP1_OUT_IRQHandler,Default_Handler
               
   .weak      OTG_HS_EP1_IN_IRQHandler            
   .thumb_set OTG_HS_EP1_IN_IRQHandler,Default_Handler
               
   .weak      OTG_HS_WKUP_IRQHandler         
   .thumb_set OTG_HS_WKUP_IRQHandler,Default_Handler
            
   .weak      OTG_HS_IRQHandler      
   .thumb_set OTG_HS_IRQHandler,Default_Handler
                  
   .weak      DCMI_IRQHandler            
   .thumb_set DCMI_IRQHandler,Default_Handler  

   .weak      FPU_IRQHandler                  
   .thumb_set FPU_IRQHandler,Default_Handler  

   .weak      TRNG_IRQHandler                  
   .thumb_set TRNG_IRQHandler,Default_Handler  

   .weak      UART6_IRQHandler                  
   .thumb_set UART6_IRQHandler,Default_Handler  

   .weak      UART7_IRQHandler                  
   .thumb_set UART7_IRQHandler,Default_Handler  

   .weak      SPI3_IRQHandler            
   .thumb_set SPI3_IRQHandler,Default_Handler

   .weak      SPI4_IRQHandler            
   .thumb_set SPI4_IRQHandler,Default_Handler

   .weak      SPI5_IRQHandler            
   .thumb_set SPI5_IRQHandler,Default_Handler

   .weak      TLI_IRQHandler            
   .thumb_set TLI_IRQHandler,Default_Handler

   .weak      TLI_ER_IRQHandler            
   .thumb_set TLI_ER_IRQHandler,Default_Handler
   
   .weak      IPA_IRQHandler            
   .thumb_set IPA_IRQHandler,Default_Handler
 