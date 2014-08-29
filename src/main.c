/**
 * A simple example
 *
 * Copyright 2014 Jari Ojanen
 */
#include "stm32f0xx.h"
#include "stlinky.h"
#include "config.h"

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

void exti_init(void)
{
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);
}

int main(void)
{
	delayVar = 200;
	stlinky_init();

	config_port_init();

	if (SysTick_Config(SystemCoreClock / 1000)) { 
		/* Capture error */ 
		while (1);
	}

	stlinky_tx(&sterm, "Hi\n", 3);
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
	delayVar--;
	if (delayVar == 500) {
		set_LED2;
	}
	else if (delayVar == 100) {
		clr_LED2;
		delayVar = 1000;
	}
}
