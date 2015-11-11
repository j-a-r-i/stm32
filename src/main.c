/**
 * A simple example
 *
 * Copyright 2014 Jari Ojanen
 */
#include "hw.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_pwr.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_syscfg.h"
#include "config.h"
#include "ds1820.h"
#include "lcd.h"
#include "main.h"
#include "test.h"
#include "out.h"
#include "queue.h"
//#include "pff.h"
#include "usart.h"
#include "nexa.h"

typedef void (*callback_fn)(void);

static __IO uint32_t delayVar;
static callback_fn timerCb;
static uint32_t    timerCounter;
static uint16_t    timer2Counter;
static RTC_TimeTypeDef timeRtc;
//static uint8_t     f_delay;
static uint8_t  measCounter;

#define EVENTS_SIZE 4
static queue_t     events;
static uint8_t     eventsData[EVENTS_SIZE];

#define EV_TIMER1 0xA1

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
	/*uint32_t count = (uint32_t)msDelay * 8;
	uint32_t i;

	for (i=0; i<count; i++) {
		//__asm__ __volatile__ ("nop");
		asm("nop");
	}*/
    uint32_t cnt;  
    SysTick->LOAD=msDelay*4;  
    SysTick->VAL=0x00;  
    SysTick->CTRL=0x01;  
    do {  
        cnt=SysTick->CTRL;  
    }  
    while(cnt & 0x01 && !(cnt & 0x10000));  
    SysTick->CTRL=0x00;  
    SysTick->VAL=0x00;
}

void delay_ms(uint16_t msDelay)  
{  
	uint32_t count = (uint32_t)msDelay * 8000;
	uint32_t i;

	for (i=0; i<count; i++) {
		//__asm__ __volatile__ ("nop");
		__asm { nop }
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
void exti_init(void)
{
	EXTI_InitTypeDef extiInit;
	NVIC_InitTypeDef nvicInit;
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	extiInit.EXTI_Line = EXTI_Line0;
	extiInit.EXTI_Mode = EXTI_Mode_Interrupt;
	extiInit.EXTI_Trigger = EXTI_Trigger_Rising;
	extiInit.EXTI_LineCmd = ENABLE;
	EXTI_Init(&extiInit);

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

	initRtc.RTC_AsynchPrediv = 99;  // LSI is 40kHz
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
void tim_init(void)
{

	TIM_TimeBaseInitTypeDef timInit;
	NVIC_InitTypeDef        nvicInit;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructInit(&timInit);
	timInit.TIM_Period = 10000;  // ms  = 10 sec
	timInit.TIM_Prescaler = (uint16_t) (SystemCoreClock / 1000) - 1;
	TIM_TimeBaseInit(TIM2, &timInit);

	nvicInit.NVIC_IRQChannel = TIM2_IRQn;
	nvicInit.NVIC_IRQChannelPriority = 3;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicInit);
 
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

#if 0
int write_sdcard(void)
{
	FATFS fs;          /* Work area (file system object) for the volume */
    BYTE buff[16];     /* File read buffer */
    UINT br;           /* File read count */
    FRESULT res;       /* Petit FatFs function common result code */

    res = pf_mount(&fs);
    if (res) 
		return res;

    res = pf_open("srcfile.dat");
    if (res)
		return res;

	res = pf_write(buff, 16, &br);
	
	return res;
}
#endif

//------------------------------------------------------------------------------
int main(void)
{
	//uint32_t loopCount;
	measCounter = 0;
	
	queue_init(&events, EVENTS_SIZE, eventsData);
#if 0
	queue_push(&events, 10);
	queue_push(&events, 20);
	queue_push(&events, 30);
	queue_push(&events, 40);
	queue_push(&events, 50);
	i = queue_pop(&events);
	i = queue_pop(&events);
	i = queue_pop(&events);
	i = queue_pop(&events);
	i = queue_pop(&events);
	delayVar = i;
#endif

	//loopCount = 0;
	delayVar = 2000;
	timerCounter = 0;
	timer2Counter = 0;

	SystemCoreClockUpdate();

	port_init();
	usart_init();
	tim_init();
  rtc_init();
	lcd_init();
	ds1820_init(PIN_TEMP1);

	if (SysTick_Config(SystemCoreClock / 1000)) { 
		/* Capture error */ 
		while (1);
	}

	usart_str("Hello\r\n");

	//write_sdcard();
	nexa_send(NEXA_CH_1, NEXA_UNIT_1, NEXA_ON);

	while (1) {
		if (queue_empty(events)) {
			delay_ms(2);
		}
		else {
			uint8_t ev = queue_pop(&events);
			uint8_t temp;
			switch (ev) {
			case EV_TIMER1:
				set_LED1;
				
				RTC_GetTime(RTC_Format_BIN, &timeRtc);
				ds1820_measure(PIN_TEMP2);
				delay_ms(750);
				temp = ds1820_read_temp(PIN_TEMP2);
				measCounter++;
			
				//out_time(usart_put, &timeRtc);
				usart_put('M');
				out_byte(usart_put, measCounter);
				usart_put(' ');
				out_byte(usart_put, temp);
				usart_put(' ');
				out_byte(usart_put, 0);
				usart_put(' ');
				out_byte(usart_put, 0);
				usart_put('\r');
				usart_put('\n');
				
				lcd_clear();
				out_time(lcd_write, &timeRtc);
				lcd_write(' ');
				out_byte(lcd_write, (temp >> 1));
				lcd_write('.');
				if (temp & 1)
					lcd_write('5');
				else
					lcd_write('0');

				clr_LED1;
				//usart_num(timeRtc.RTC_Hours);
				//usart_str(":");
				//usart_num(timeRtc.RTC_Minutes);
				//usart_str(":");
				//usart_num(timeRtc.RTC_Seconds);
				//usart_str("\r\n");
				//delay_ms(30);
				break;
			}
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
		//if (timer2Counter == 0)
			//f_delay = 0;
	}
}

//------------------------------------------------------------------------------
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET) {
		//USART_SendData(USART1, 'a');
		usart_str("rtc irq\n");
		RTC_ClearITPendingBit(RTC_IT_ALRA);
	}
}

//------------------------------------------------------------------------------
void TIM2_IRQHandler(void) 
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		queue_push(&events, EV_TIMER1);
#if 0
		toggle_LED2;
#endif	
	}
}

