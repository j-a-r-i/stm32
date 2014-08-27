/**
 * A simple example
 *
 * Copyright 2014 Jari Ojanen
 */
#include "stm32f0xx.h"
#include "stlinky.h"

#define BSRR_VAL        0x0300

GPIO_InitTypeDef               gpioInit;
static volatile struct stlinky sterm;
static __IO uint32_t delayVar;

void delay (int a);
void stlinky_init(void);
void SysTick_Handler(void);


void stlinky_init(void)
{
	sterm.magic = STLINKY_MAGIC;
	sterm.bufsize = CONFIG_LIB_STLINKY_BSIZE;
	sterm.txsize = 0;
	sterm.rxsize = 0;
}


int main(void)
{
	delayVar = 200;
	stlinky_init();

	/* GPIOC Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	/* Configure PC8 and PC9 in output pushpull mode */
	gpioInit.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpioInit.GPIO_Mode = GPIO_Mode_OUT;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &gpioInit);

	if (SysTick_Config(SystemCoreClock / 1000)) { 
		/* Capture error */ 
		while (1);
	}

	stlinky_tx(&sterm, "Hi\n", 3);
	while (1) {
		//stlinky_tx(&sterm, "Hi", 2);
		//GPIOC->BSRR = BSRR_VAL;
		//delay(900000);
		//GPIOC->BRR = BSRR_VAL;
		//delay(900000);
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
	delayVar--;
	if (delayVar == 500) {
		GPIOC->BSRR = BSRR_VAL;
	}
	else if (delayVar == 100) {
		GPIOC->BRR = BSRR_VAL;
		delayVar = 1000;
	}
}
