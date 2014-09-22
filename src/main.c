/**
 * A simple example
 *
 * Copyright 2014 Jari Ojanen
 */
#include "stm32f0xx.h"
//#include "stlinky.h"
#include "config.h"
#include "ds1820.h"
#include "lcd.h"
//#include <stdio.h>
#include "main.h"
#include "test.h"

typedef void (*callback_fn)(void);

//static volatile struct stlinky sterm;
static __IO uint32_t delayVar;
static callback_fn timerCb;
static uint32_t    timerCounter;
static uint16_t    timer2Counter;
static RTC_TimeTypeDef timeRtc;
static uint8_t     tim2Counter;
static uint8_t     f_print;  // interrupt tells mainloop to do printing
static uint8_t     f_delay;

void SysTick_Handler(void);
void usart_str(const char *s);

//------------------------------------------------------------------------------
void delayCb_ms(uint32_t msDelay, callback_fn fn)
{
	timerCb = fn;
	timerCounter = msDelay;
}

void delay_us(uint16_t msDelay)  
{
	uint32_t count = (uint32_t)msDelay * 10;
	uint32_t i;

	for (i=0; i<count; i++) {
		//__asm__ __volatile__ ("nop");
		asm("nop");
	}
}

void delay_ms(uint16_t msDelay)  
{  
	uint32_t count = (uint32_t)msDelay * 8000;
	uint32_t i;

	for (i=0; i<count; i++) {
		__asm__ __volatile__ ("nop");
		//asm("nop");
	}

    /*uint32_t temp;  
    SysTick->LOAD=((uint32_t)msDelay)*48000;  
    SysTick->VAL=0x00;  
    SysTick->CTRL=0x01;  
    do  
    {  
        temp=SysTick->CTRL;  
    }  
    while(temp&0x01 && !(temp&0x10000));  
    SysTick->CTRL=0x00;  
    SysTick->VAL=0x00;
	*/
}  

//------------------------------------------------------------------------------
/*void delay_ms2(uint16_t msDelay)
{
    uint32_t i = 0x20000;

	i = msDelay*i;

	while (i)
		i--;

	timer2Counter = msDelay;
	f_delay = 1;

	while (f_delay) {
		//	if (timer2Counter % 2)
		//	set_LED2;
		//else
		//clr_LED2;
   }
}*/


//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void rtc_init(void)
{
	RTC_InitTypeDef initRtc;
	RTC_AlarmTypeDef alarmRtc;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	PWR_BackupAccessCmd(ENABLE);

	//RCC_BackupResetCmd(ENABLE);
    //RCC_BackupResetCmd(DISABLE);

	RCC_LSICmd(ENABLE);

	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

	RCC_RTCCLKCmd(ENABLE);

	if (RTC_WaitForSynchro() == ERROR)
		usart_str("E RTC 0\r\n");

	//initRtc.RTC_AsynchPrediv = 0x18f;
	//initRtc.RTC_SynchPrediv  = 0x63;
	initRtc.RTC_AsynchPrediv = 99;
	initRtc.RTC_SynchPrediv  = 399;
	initRtc.RTC_HourFormat  = RTC_HourFormat_24;
	if (RTC_Init(&initRtc) == ERROR)
		usart_str("E RTC 1\r\n");

	// Sets time
	//
	timeRtc.RTC_H12  = RTC_H12_AM;
	timeRtc.RTC_Hours = 12;
	timeRtc.RTC_Minutes = 00;
	timeRtc.RTC_Seconds = 00;
	if (RTC_SetTime(RTC_Format_BIN, &timeRtc))
		usart_str("E RTC 2\r\n");

	// Configure alarm
	//
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
	alarmRtc.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
	alarmRtc.RTC_AlarmTime.RTC_Hours = 12;
	alarmRtc.RTC_AlarmTime.RTC_Minutes = 00;
	alarmRtc.RTC_AlarmTime.RTC_Seconds = 20;
	alarmRtc.RTC_AlarmDateWeekDay = 0x31;
	alarmRtc.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	alarmRtc.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &alarmRtc);

	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

//------------------------------------------------------------------------------
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

	//USART_ITConfig(USART1, USART_IT_TXNE, ENABLE);
    //NVIC_EnableIRQ(USART1_IRQn);

	NVIC_InitTypeDef nvicInit;
	nvicInit.NVIC_IRQChannel = USART1_IRQn;
	nvicInit.NVIC_IRQChannelPriority = 0x02;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicInit);

	USART_Cmd(USART1, ENABLE);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

//------------------------------------------------------------------------------
void usart_put(uint8_t ch)
{
      while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

      USART_SendData(USART1, (uint8_t) ch);
}

//------------------------------------------------------------------------------
void usart_str(const char *s)
{
	while(*s) {
		while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
		USART_SendData(USART1, *s);
		s++;
	}
}

//------------------------------------------------------------------------------
void usart_num(uint8_t n)
{
	char digit0, digit1, digit2;

	digit0 = '0'+(n % 10);
	n /= 10;
	digit1 = '0'+(n % 10);

	if (n > 100) {
		n /= 10;
		digit2 = '0'+(n % 10);

		usart_put(digit2);
	}
	usart_put(digit1);
	usart_put(digit0);
}

//------------------------------------------------------------------------------
void tim_init(void)
{

	TIM_TimeBaseInitTypeDef timInit;
	NVIC_InitTypeDef        nvicInit;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructInit(&timInit);
	timInit.TIM_Period = 1000;  // ms
	timInit.TIM_Prescaler = (uint16_t) (SystemCoreClock / 1000) - 1;
	TIM_TimeBaseInit(TIM2, &timInit);

	nvicInit.NVIC_IRQChannel = TIM2_IRQn;
	nvicInit.NVIC_IRQChannelPriority = 3;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicInit);
 
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

//------------------------------------------------------------------------------
int main(void)
{
	//uint32_t loopCount;

	//loopCount = 0;
	delayVar = 2000;
	timerCounter = 0;
	timer2Counter = 0;

	SystemCoreClockUpdate();

	config_port_init();
	usart_init();
	tim_init();
  	rtc_init();
	lcd_init();
	ds1820_init(PIN_TEMP1);

	if (SysTick_Config(SystemCoreClock / 1000)) { 
		/* Capture error */ 
		while (1);
	}


	usart_str("\r\nHello\r\n");

	lcd_str("hello");

	ds1820_read_temp(PIN_TEMP1);


	/*while (1) {
		set_LED2;
		delay_ms2(1000);
		clr_LED2;
		delay_ms2(1000);
    }*/

	//	stlinky_tx(&sterm, "Hi\n", 3);
	while (1) {
		//stlinky_tx(&sterm, "Hi", 2);
		if (f_print) {
			f_print = 0;

			RTC_GetTime(RTC_Format_BIN, &timeRtc);
			usart_num(timeRtc.RTC_Hours);
			usart_str(":");
			usart_num(timeRtc.RTC_Minutes);
			usart_str(":");
			usart_num(timeRtc.RTC_Seconds);
			usart_str("\r\n");
		}
#if 0
		loopCount++;
		if (loopCount == 1)
			set_LED2;
		else if (loopCount == 0x100000)
			clr_LED2;
		else if (loopCount == 0x200000)
			loopCount=0;
#endif
	}
}

//------------------------------------------------------------------------------
void SysTick_Handler(void)
{
	if (timerCounter > 0) {
		timerCounter--;
		if (timerCounter == 0)
			(*timerCb)();
	}

	if (timer2Counter > 0) {
		timer2Counter--;
		if (timer2Counter == 0)
			f_delay = 0;
	}
}

//------------------------------------------------------------------------------
void RTC_IRQHandler(void)
{
	//set_LED1;
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET) {
		USART_SendData(USART1, 'a');
		RTC_ClearITPendingBit(RTC_IT_ALRA);
	}
}

//------------------------------------------------------------------------------
void TIM2_IRQHandler(void) 
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		f_print = 1;
		tim2Counter++;
		/*if (tim2Counter % 2)
			set_LED1;
		else
			clr_LED1;
		*/
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

//------------------------------------------------------------------------------
void USART1_IRQHandler(void) 
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) { // Received characters modify string
		uint8_t ch = USART1->RDR;
		test_exec(ch);
	}
}
