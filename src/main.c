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

typedef void (*callback_fn)(void);

//static volatile struct stlinky sterm;
static __IO uint32_t delayVar;
static callback_fn timerCb;
static uint32_t    timerCounter;
static uint8_t     tmp;
static RTC_TimeTypeDef timeRtc;
static uint8_t     tim2Counter;

void delay (int a);
//void stlinky_init(void);
void SysTick_Handler(void);
void usart_str(const char *s);

//------------------------------------------------------------------------------
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

	//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //NVIC_EnableIRQ(USART1_IRQn);

	USART_Cmd(USART1, ENABLE);
}

//------------------------------------------------------------------------------
void usart_put(uint8_t ch)
{
      USART_SendData(USART1, (uint8_t) ch);

      while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
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
	delayVar = 2000;
	timerCounter = 0;
	tmp = 0;
	//	stlinky_init();

	SystemCoreClockUpdate();

	config_port_init();
	usart_init();
	rtc_init();
	lcd_init();
	tim_init();
	ds1820_init(PIN_TEMP1);

	if (SysTick_Config(SystemCoreClock / 1000)) { 
		/* Capture error */ 
		while (1);
	}

	usart_str("\r\nHello\r\n");
	//printf("Hello\n");


	lcd_write('h');
	lcd_write('i');
	ds1820_read_temp(PIN_TEMP1);

	//	stlinky_tx(&sterm, "Hi\n", 3);
	while (1) {
		//stlinky_tx(&sterm, "Hi", 2);
	}
}

//------------------------------------------------------------------------------
int __io_putchar(int ch)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

	return ch;
}

//------------------------------------------------------------------------------
void delay (int a)
{
	volatile int i,j;
	
	for (i=0 ; i < a ; i++) { 
		j++;
	}	
	return;
}

//------------------------------------------------------------------------------
void SysTick_Handler(void)
{
	if (timerCounter > 0) {
		timerCounter--;
		if (timerCounter == 0)
			(*timerCb)();
	}

	delayVar--;
	if (delayVar == 1000) {
		config_port_set(PIN_LED2);
		//set_LED2;
	}
	else if (delayVar == 0) {
		config_port_clear(PIN_LED2);
		//clr_LED2;
		delayVar = 2000;
		//USART_SendData(USART1, 'T');
		tmp++;

		RTC_GetTime(RTC_Format_BIN, &timeRtc);
		usart_num(timeRtc.RTC_Hours);
		//usart_num(tmp);
		usart_str(":");
		usart_num(timeRtc.RTC_Minutes);
		usart_str(":");
		usart_num(timeRtc.RTC_Seconds);
		usart_str("\r\n");
	}
}

//------------------------------------------------------------------------------
void RTC_IRQHandler(void)
{
	set_LED1;
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET) {
		USART_SendData(USART1, 'a');
		RTC_ClearITPendingBit(RTC_IT_ALRA);
	}
}

//------------------------------------------------------------------------------
void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		tim2Counter++;
		if (tim2Counter % 2)
			set_LED1;
		else
			clr_LED1;

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
