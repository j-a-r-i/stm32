/**
 * A simple example
 *
 * Copyright 2014 Jari Ojanen
 */
#include "stm32f0xx.h"
//#include "stlinky.h"
#include "config.h"

typedef void (*callback_fn)(void);

//static volatile struct stlinky sterm;
static __IO uint32_t delayVar;
static callback_fn timerCb;
static uint32_t    timerCounter;

void delay (int a);
//void stlinky_init(void);
void SysTick_Handler(void);

void delay_ms(uint32_t msDelay, callback_fn fn)
{
	timerCb = fn;
	timerCounter = msDelay;
}

/*void stlinky_init(void)
{
	sterm.magic = STLINKY_MAGIC;
	sterm.bufsize = CONFIG_LIB_STLINKY_BSIZE;
	sterm.txsize = 0;
	sterm.rxsize = 0;
	}*/

void exti_init(void)
{
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	EXTI_InitTypeDef extiInit;
	EXTI_StructInit(&extiInit);
	extiInit.EXTI_Line = EXTI_Line0;
	extiInit.EXTI_Mode = EXTI_Mode_Interrupt;
	extiInit.EXTI_Trigger = EXTI_Trigger_Rising;
	extiInit.EXTI_LineCmd = ENABLE;
	EXTI_Init(&extiInit);

	NVIC_InitTypeDef nvicInit;
	nvicInit.NVIC_IRQChannel = EXTI0_1_IRQn;
	nvicInit.NVIC_IRQChannelPriority = 0x01;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicInit);
}

void rtc_init(void)
{
	RTC_InitTypeDef initRtc;
    RTC_TimeTypeDef timeRtc;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);

	PWR_BackupAccessCmd(ENABLE);

	RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);

	//RCC_LSEConfig(RCC_LSI_ON);

	//while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
}

int main(void)
{
	delayVar = 200;
	timerCounter = 0;

	//	stlinky_init();

	config_port_init();

	if (SysTick_Config(SystemCoreClock / 1000)) { 
		/* Capture error */ 
		while (1);
	}

	//	stlinky_tx(&sterm, "Hi\n", 3);
	while (1) {
		//stlinky_tx(&sterm, "Hi", 2);
	}
}

void delay (int a)
{
	volatile int i,j;
	
	for (i=0 ; i < a ; i++) { 
		j++;
	}	
	return;
}

void SysTick_Handler(void)
{
	if (timerCounter > 0) {
		timerCounter--;
		if (timerCounter == 0)
			(*timerCb)();
	}

	delayVar--;
	if (delayVar == 500) {
		set_LED2;
	}
	else if (delayVar == 100) {
		clr_LED2;
		delayVar = 1000;
	}
}
