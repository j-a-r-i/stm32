#include "stm32f0xx.h"
#include "out.h"

//------------------------------------------------------------------------------
void out_str(FnPut put, const char* str)
{
	while(*str) {
		(*put)(*str);
		str++;
	}
}

//------------------------------------------------------------------------------
void out_byte(FnPut put, uint8_t num)
{
	char digit0, digit1, digit2;

	digit0 = '0'+(num % 10);
	num /= 10;
	digit1 = '0'+(num % 10);

	if (num > 100) {
		num /= 10;
		digit2 = '0'+(num % 10);

		(*put)(digit2);
	}
	(*put)(digit1);
	(*put)(digit0);
}

//------------------------------------------------------------------------------
void out_time(FnPut put, RTC_TimeTypeDef *time)
{
	out_byte(put, time->RTC_Hours);
	(*put)(':');
	out_byte(put, time->RTC_Minutes);
	//	(*put)(':');
	//	out_byte(put, time->RTC_Seconds);
	//out_str(put, "\r\n");	
}
