#include <algorithm>
#include <cstdint>

#include "gd32f4xx.h"

extern "C" {
extern void __libc_init_array();
extern void __libc_fini_array();

extern void SystemInit();
extern int main();

extern unsigned int _sidata;
extern unsigned int _sdata;
extern unsigned int _edata;

extern unsigned int _sbss;
extern unsigned int _ebss;

extern unsigned int _estack;
}

namespace {
	__attribute__((noinline)) auto setUp() -> void {
		std::copy(&_sidata, &_sidata + (&_edata - &_sdata), &_sdata);
		std::fill(&_sbss, &_ebss, 0);

		__libc_init_array();

		SystemInit();
	}

	[[noreturn]] __attribute__((noinline)) auto tearDown() -> void {
		__libc_fini_array();
		while (true) {}
	}
	[[noreturn]] __attribute__((section(".after_vectors"), naked)) auto startup() -> void {
		setUp();

		asm volatile("ldr r0, = main");
		asm volatile("blx r0");

		tearDown();
	}
}  // namespace

extern "C" {
void __attribute__((section(".after_vectors"), weak, naked)) Reset_Handler() {
	startup();
}

void __attribute__((weak)) Default_Handler() {
	while (true) {}
}

void __attribute__((weak, alias("Default_Handler"), nothrow)) NMI_Handler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) HardFault_Handler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) MemManage_Handler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) BusFault_Handler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) UsageFault_Handler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) SVC_Handler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DebugMon_Handler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) PendSV_Handler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) SysTick_Handler();

void __attribute__((weak, alias("Default_Handler"), nothrow)) WWDG_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) LVD_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TAMP_STAMP_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) RTC_WKUP_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) FLASH_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) RCC_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) EXTI0_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) EXTI1_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) EXTI2_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) EXTI3_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) EXTI4_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA0_Channel0_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA0_Channel1_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA0_Channel2_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA0_Channel3_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA0_Channel4_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA0_Channel5_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA0_Channel6_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) ADC_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) CAN0_TX_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) CAN0_RX0_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) CAN0_RX1_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) CAN0_SCE_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) EXTI9_5_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER0_BRK_TIMER8_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER0_UP_TIMER9_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER0_TRG_CMT_TIMER10_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER0_Channel_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER1_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER2_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER3_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) I2C0_EV_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) I2C0_ER_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) I2C1_EV_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) I2C1_ER_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) SPI0_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) SPI1_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) USART0_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) USART1_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) USART2_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) EXTI15_10_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) RTC_Alarm_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) OTG_FS_WKUP_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER7_BRK_TIMER11_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER7_UP_TIMER12_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER7_TRG_CMT_TIMER13_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER7_Channel_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA0_Channel7_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) EXMC_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) SDIO_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER4_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) SPI2_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) UART3_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) UART4_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER5_DAC_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TIMER6_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA1_Channel0_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA1_Channel1_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA1_Channel2_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA1_Channel3_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA1_Channel4_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) ENET_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) ENET_WKUP_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) CAN1_TX_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) CAN1_RX0_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) CAN1_RX1_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) CAN1_SCE_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) OTG_FS_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA1_Channel5_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA1_Channel6_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DMA1_Channel7_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) USART5_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) I2C2_EV_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) I2C2_ER_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) OTG_HS_EP1_OUT_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) OTG_HS_EP1_IN_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) OTG_HS_WKUP_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) OTG_HS_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) DCMI_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TRNG_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) FPU_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) UART6_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) UART7_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) SPI3_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) SPI4_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) SPI5_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TLI_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) TLI_ER_IRQHandler();
void __attribute__((weak, alias("Default_Handler"), nothrow)) IPA_IRQHandler();

// ----------------------------------------------------------------------------

using pHandler = void (*)();

// ----------------------------------------------------------------------------

// The vector table.
// This relies on the linker script to place at correct location in memory.
__attribute__((section(".isr_vector"), used)) pHandler g_pfnVectors[] = {
	reinterpret_cast<pHandler>(&_estack),
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0,
	0,
	0,
	0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler,
	WWDG_IRQHandler,
	LVD_IRQHandler,
	TAMP_STAMP_IRQHandler,
	RTC_WKUP_IRQHandler,
	FLASH_IRQHandler,
	RCC_IRQHandler,
	EXTI0_IRQHandler,
	EXTI1_IRQHandler,
	EXTI2_IRQHandler,
	EXTI3_IRQHandler,
	EXTI4_IRQHandler,
	DMA0_Channel0_IRQHandler,
	DMA0_Channel1_IRQHandler,
	DMA0_Channel2_IRQHandler,
	DMA0_Channel3_IRQHandler,
	DMA0_Channel4_IRQHandler,
	DMA0_Channel5_IRQHandler,
	DMA0_Channel6_IRQHandler,
	ADC_IRQHandler,
	CAN0_TX_IRQHandler,
	CAN0_RX0_IRQHandler,
	CAN0_RX1_IRQHandler,
	CAN0_SCE_IRQHandler,
	EXTI9_5_IRQHandler,
	TIMER0_BRK_TIMER8_IRQHandler,
	TIMER0_UP_TIMER9_IRQHandler,
	TIMER0_TRG_CMT_TIMER10_IRQHandler,
	TIMER0_Channel_IRQHandler,
	TIMER1_IRQHandler,
	TIMER2_IRQHandler,
	TIMER3_IRQHandler,
	I2C0_EV_IRQHandler,
	I2C0_ER_IRQHandler,
	I2C1_EV_IRQHandler,
	I2C1_ER_IRQHandler,
	SPI0_IRQHandler,
	SPI1_IRQHandler,
	USART0_IRQHandler,
	USART1_IRQHandler,
	USART2_IRQHandler,
	EXTI15_10_IRQHandler,
	RTC_Alarm_IRQHandler,
	OTG_FS_WKUP_IRQHandler,
	TIMER7_BRK_TIMER11_IRQHandler,
	TIMER7_UP_TIMER12_IRQHandler,
	TIMER7_TRG_CMT_TIMER13_IRQHandler,
	TIMER7_Channel_IRQHandler,
	DMA0_Channel7_IRQHandler,
	EXMC_IRQHandler,
	SDIO_IRQHandler,
	TIMER4_IRQHandler,
	SPI2_IRQHandler,
	UART3_IRQHandler,
	UART4_IRQHandler,
	TIMER5_DAC_IRQHandler,
	TIMER6_IRQHandler,
	DMA1_Channel0_IRQHandler,
	DMA1_Channel1_IRQHandler,
	DMA1_Channel2_IRQHandler,
	DMA1_Channel3_IRQHandler,
	DMA1_Channel4_IRQHandler,
	ENET_IRQHandler,
	ENET_WKUP_IRQHandler,
	CAN1_TX_IRQHandler,
	CAN1_RX0_IRQHandler,
	CAN1_RX1_IRQHandler,
	CAN1_SCE_IRQHandler,
	OTG_FS_IRQHandler,
	DMA1_Channel5_IRQHandler,
	DMA1_Channel6_IRQHandler,
	DMA1_Channel7_IRQHandler,
	USART5_IRQHandler,
	I2C2_EV_IRQHandler,
	I2C2_ER_IRQHandler,
	OTG_HS_EP1_OUT_IRQHandler,
	OTG_HS_EP1_IN_IRQHandler,
	OTG_HS_WKUP_IRQHandler,
	OTG_HS_IRQHandler,
	DCMI_IRQHandler,
	0,
	TRNG_IRQHandler,
	FPU_IRQHandler,
	UART6_IRQHandler,
	UART7_IRQHandler,
	SPI3_IRQHandler,
	SPI4_IRQHandler,
	SPI5_IRQHandler,
	0,
	TLI_IRQHandler,
	TLI_ER_IRQHandler,
	IPA_IRQHandler
};
}