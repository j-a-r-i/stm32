#include "stm32f0xx.h"
#include "config.h"

void config_port_init (void)
{
	GPIO_InitTypeDef ioInit;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	ioInit.GPIO_Pin = GPIO_Pin_0;
	ioInit.GPIO_Mode = GPIO_Mode_IN;
	ioInit.GPIO_OType = GPIO_OType_PP;
	ioInit.GPIO_PuPd = GPIO_PuPd_DOWN;
	ioInit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &ioInit);
	
	ioInit.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	ioInit.GPIO_Mode = GPIO_Mode_AF;
	ioInit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &ioInit);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource14, 0);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, 0);
	
	ioInit.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	ioInit.GPIO_Mode = GPIO_Mode_AF;
	ioInit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &ioInit);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, 0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, 0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, 0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, 0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, 0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, 0);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	ioInit.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	ioInit.GPIO_Mode = GPIO_Mode_OUT;
	ioInit.GPIO_OType = GPIO_OType_PP;
	ioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ioInit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &ioInit);
}
