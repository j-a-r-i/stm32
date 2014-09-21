#include "stm32f0xx.h"
#include "test.h"
#include "config.h"

static uint32_t bits = 0;

#define BIT1 1
#define BIT2 2
#define BIT3 4
#define BIT4 8
#define BIT5 16
#define BIT6 32
#define BIT7 64
#define BIT8 128
#define BIT9 256
#define BIT10 512
#define BIT11 1024
#define BIT12 2048
#define BIT13 4096
#define BIT14 8192
#define BIT15 16384
#define BIT16 32768
#define BIT17 65536
#define BIT18 131072
#define BIT19 262144
#define BIT20 524288

//------------------------------------------------------------------------------
void test_setbit(uint8_t pin, uint32_t bit)
{
	if (bits & bit) {
		bits &= ~bit;
		config_port_clear(pin);
	}
	else {
		bits |= bit;
		config_port_set(pin);
	}
}

//------------------------------------------------------------------------------
void test_exec(uint8_t cmd)
{
	switch (cmd) {
	case 'a':  test_setbit(PIN_LCD_DATA1,  BIT1);  break;
	case 'b':  test_setbit(PIN_LCD_DATA2,  BIT2);  break;
	case 'c':  test_setbit(PIN_LCD_DATA3,  BIT3);  break;
	case 'd':  test_setbit(PIN_LCD_DATA4,  BIT4);  break;
	case 'e':  test_setbit(PIN_LCD_RS,     BIT5);  break;
	case 'f':  test_setbit(PIN_LCD_ENABLE, BIT6);  break;
	case 'g':  test_setbit(PIN_LED1,       BIT7);  break;
	case 'h':  test_setbit(PIN_LED2,       BIT8);  break;
	}
}		
