#include "stm32f0xx.h"
#include "config.h"

void config_port_init (void)
{
	GPIO_InitTypeDef ioInit;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	ioInit.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	ioInit.GPIO_Mode = GPIO_Mode_OUT;
	ioInit.GPIO_OType = GPIO_OType_PP;
	ioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ioInit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &ioInit);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	ioInit.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	ioInit.GPIO_Mode = GPIO_Mode_OUT;
	ioInit.GPIO_OType = GPIO_OType_PP;
	ioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ioInit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &ioInit);
}
