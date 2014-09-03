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

void usart_init(void)
{
	USART_InitTypeDef initUsart;
	USART_ClockInitTypeDef clockInit;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_ClockStructInit(&clockInit);
	USART_ClockInit(USART1, &clockInit);


	initUsart.USART_BaudRate = 9600;
	initUsart.USART_WordLength = USART_WordLength_8b;
	initUsart.USART_StopBits = USART_StopBits_1;
	initUsart.USART_Parity = USART_Parity_No;
	initUsart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	initUsart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &initUsart);

	//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //NVIC_EnableIRQ(USART1_IRQn);

	USART_Cmd(USART1, ENABLE);
}

void usart_put(uint8_t ch)
{
      USART_SendData(USART1, (uint8_t) ch);

      while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {
      }
}

void usart_str(const char *s)
{
	while(*s) {
		while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
		USART_SendData(USART1, *s);
		*s++;
	}
}

int main(void)
{
	delayVar = 2000;
	timerCounter = 0;

	//	stlinky_init();

	config_port_init();
	usart_init();

	if (SysTick_Config(SystemCoreClock / 1000)) { 
		/* Capture error */ 
		while (1);
	}

	usart_str("\nHello\n");

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
	if (delayVar == 1000) {
		set_LED2;
	}
	else if (delayVar == 0) {
		clr_LED2;
		delayVar = 2000;
		USART_SendData(USART1, 'T');
	}
}
