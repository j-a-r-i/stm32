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
//#include "pff.h"
#include "usart.h"
#include "nexa.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static RTC_TimeTypeDef timeRtc;
static QueueHandle_t commQueue1;

#define EV_TIMER1 0xA1

void usart_str(const char *s);


//------------------------------------------------------------------------------
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

	if (RTC_WaitForSynchro() == ERROR) {
#ifdef USE_SERIAL
		usart_str("E RTC 0\r\n");
#endif
	}

	initRtc.RTC_AsynchPrediv = 99;  // LSI is 40kHz
	initRtc.RTC_SynchPrediv  = 399;
	initRtc.RTC_HourFormat  = RTC_HourFormat_24;
	if (RTC_Init(&initRtc) == ERROR) {
#ifdef USE_SERIAL
		usart_str("E RTC 1\r\n");
#endif
	}

	// Sets time
	//
	timeRtc.RTC_H12  = RTC_H12_AM;
	timeRtc.RTC_Hours = 12;
	timeRtc.RTC_Minutes = 00;
	timeRtc.RTC_Seconds = 00;
	if (RTC_SetTime(RTC_Format_BIN, &timeRtc)) {
#ifdef USE_SERIAL
		usart_str("E RTC 2\r\n");
#endif
	}

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
void TaskComm(void* pvParameters)
{		
	char ch = 'a';
	(void) pvParameters;                    // Just to stop compiler warnings.
	
#ifdef USE_SERIAL
	usart_str("Hello\r\n");
#endif
	for (;;) {
		//vTaskDelay(500);
		if (xQueueReceive(commQueue1, &ch, (TickType_t)1000)) {
#ifdef USE_SERIAL
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
#endif
		}
	}
}

//------------------------------------------------------------------------------
void TaskMeasure(void* pvParameters)
{		
		uint8_t temp;
		uint8_t measCounter = 0;

   	(void) pvParameters;                    // Just to stop compiler warnings.
	
    for (;;) {
				RTC_GetTime(RTC_Format_BIN, &timeRtc);
				ds1820_measure(PIN_TEMP2);
				vTaskDelay(750 / portTICK_PERIOD_MS);  // 750ms delay
				temp = ds1820_read_temp(PIN_TEMP2);
				measCounter++;

				vTaskDelay(10000 / portTICK_PERIOD_MS);  // 10 sec delay
    }
}



//------------------------------------------------------------------------------
int main(void)
{
	SystemCoreClockUpdate();

	port_init();
#ifdef USE_SERIAL
	usart_init();
#endif
	tim_init();
  rtc_init();
#ifdef USE_LCD
	lcd_init();
#endif
	ds1820_init(PIN_TEMP1);

	if (SysTick_Config(SystemCoreClock / 1000)) { 
		/* Capture error */ 
		while (1);
	}

	 xQueueCreate(5, sizeof(char));
	
   xTaskCreate( TaskComm,  "cmm", configMINIMAL_STACK_SIZE, NULL,
        tskIDLE_PRIORITY, ( xTaskHandle * ) NULL );
   xTaskCreate( TaskMeasure,"meas", configMINIMAL_STACK_SIZE, NULL, 
        tskIDLE_PRIORITY, ( xTaskHandle * ) NULL );

   vTaskStartScheduler();
		
	nexa_send(NEXA_CH_1, NEXA_UNIT_1, NEXA_ON);

	for (;;);
/*
	set_LED1;
#ifdef USE_LCD			
				lcd_clear();
				out_time(lcd_write, &timeRtc);
				lcd_write(' ');
				out_byte(lcd_write, (temp >> 1));
				lcd_write('.');
				if (temp & 1)
					lcd_write('5');
				else
					lcd_write('0');
#endif				
				clr_LED1;
*/
}

//------------------------------------------------------------------------------
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET) {
		//USART_SendData(USART1, 'a');
#ifdef USE_SERIAL
		usart_str("rtc irq\n");
#endif
		RTC_ClearITPendingBit(RTC_IT_ALRA);
	}
}

//------------------------------------------------------------------------------
void TIM2_IRQHandler(void) 
{
	char cIn = 'a';
	BaseType_t highPriorityTaskWoken = pdFALSE;
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		//queue_push(&events, EV_TIMER1);
		
		xQueueSendToBackFromISR(commQueue1, &cIn, &highPriorityTaskWoken);
#if 0
		toggle_LED2;
#endif	
	}
}

