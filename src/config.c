#include "hw.h"
#include "config.h"

void config_port_init (void)
{
	GPIO_InitTypeDef ioInit;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	ioInit.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_13;
	ioInit.GPIO_Mode = GPIO_Mode_OUT;
	ioInit.GPIO_OType = GPIO_OType_PP;
	ioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ioInit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &ioInit);
	
	ioInit.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_12;
	ioInit.GPIO_Mode = GPIO_Mode_IN;
	ioInit.GPIO_OType = GPIO_OType_PP;
	ioInit.GPIO_PuPd = GPIO_PuPd_DOWN;
	ioInit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &ioInit);
	
	ioInit.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10;
	ioInit.GPIO_Mode = GPIO_Mode_AF;
	ioInit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &ioInit);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, 0);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, 0);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, 0);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, 1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, 1);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	ioInit.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	ioInit.GPIO_Mode = GPIO_Mode_OUT;
	ioInit.GPIO_OType = GPIO_OType_PP;
	ioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ioInit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &ioInit);
	
	ioInit.GPIO_Pin = GPIO_Pin_6;
	ioInit.GPIO_Mode = GPIO_Mode_IN;
	ioInit.GPIO_OType = GPIO_OType_PP;
	ioInit.GPIO_PuPd = GPIO_PuPd_DOWN;
	ioInit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &ioInit);
	
	ioInit.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	ioInit.GPIO_Mode = GPIO_Mode_AF;
	ioInit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &ioInit);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, 1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, 1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, 1);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	ioInit.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13;
	ioInit.GPIO_Mode = GPIO_Mode_OUT;
	ioInit.GPIO_OType = GPIO_OType_PP;
	ioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ioInit.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &ioInit);
}

void config_port_set (uint8_t pin)
{
	switch (pin)
	{
		case PIN_BUTTON:
			GPIOA->BSRR = GPIO_Pin_0;
			break;
		case PIN_BTN1:
			GPIOA->BSRR = GPIO_Pin_1;
			break;
		case PIN_BTN2:
			GPIOA->BSRR = GPIO_Pin_2;
			break;
		case PIN_BTN3:
			GPIOA->BSRR = GPIO_Pin_3;
			break;
		case PIN_IRIN:
			GPIOA->BSRR = GPIO_Pin_8;
			break;
		case PIN_NEXA_OUT:
			GPIOA->BSRR = GPIO_Pin_11;
			break;
		case PIN_NEXA_IN:
			GPIOA->BSRR = GPIO_Pin_12;
			break;
		case PIN_IROUT:
			GPIOA->BSRR = GPIO_Pin_13;
			break;
		case PIN_CS_RFM12B:
			GPIOB->BSRR = GPIO_Pin_0;
			break;
		case PIN_CS_W5200:
			GPIOB->BSRR = GPIO_Pin_1;
			break;
		case PIN_CS_SDCARD:
			GPIOB->BSRR = GPIO_Pin_2;
			break;
		case PIN_TEMP2:
			GPIOB->BSRR = GPIO_Pin_3;
			break;
		case PIN_TEMP3:
			GPIOB->BSRR = GPIO_Pin_4;
			break;
		case PIN_TEMP4:
			GPIOB->BSRR = GPIO_Pin_5;
			break;
		case PIN_RFM12B_IRQ:
			GPIOB->BSRR = GPIO_Pin_6;
			break;
		case PIN_LCD_DATA1:
			GPIOC->BSRR = GPIO_Pin_0;
			break;
		case PIN_LCD_DATA2:
			GPIOC->BSRR = GPIO_Pin_1;
			break;
		case PIN_LCD_DATA3:
			GPIOC->BSRR = GPIO_Pin_2;
			break;
		case PIN_LCD_DATA4:
			GPIOC->BSRR = GPIO_Pin_3;
			break;
		case PIN_LED1:
			GPIOC->BSRR = GPIO_Pin_8;
			break;
		case PIN_LED2:
			GPIOC->BSRR = GPIO_Pin_9;
			break;
		case PIN_LCD_RS:
			GPIOC->BSRR = GPIO_Pin_10;
			break;
		case PIN_LCD_ENABLE:
			GPIOC->BSRR = GPIO_Pin_11;
			break;
		case PIN_TEMP1:
			GPIOC->BSRR = GPIO_Pin_13;
			break;
	}
}

void config_port_clear (uint8_t pin)
{
	switch (pin)
	{
		case PIN_BUTTON:
			GPIOA->BRR = GPIO_Pin_0;
			break;
		case PIN_BTN1:
			GPIOA->BRR = GPIO_Pin_1;
			break;
		case PIN_BTN2:
			GPIOA->BRR = GPIO_Pin_2;
			break;
		case PIN_BTN3:
			GPIOA->BRR = GPIO_Pin_3;
			break;
		case PIN_IRIN:
			GPIOA->BRR = GPIO_Pin_8;
			break;
		case PIN_NEXA_OUT:
			GPIOA->BRR = GPIO_Pin_11;
			break;
		case PIN_NEXA_IN:
			GPIOA->BRR = GPIO_Pin_12;
			break;
		case PIN_IROUT:
			GPIOA->BRR = GPIO_Pin_13;
			break;
		case PIN_CS_RFM12B:
			GPIOB->BRR = GPIO_Pin_0;
			break;
		case PIN_CS_W5200:
			GPIOB->BRR = GPIO_Pin_1;
			break;
		case PIN_CS_SDCARD:
			GPIOB->BRR = GPIO_Pin_2;
			break;
		case PIN_TEMP2:
			GPIOB->BRR = GPIO_Pin_3;
			break;
		case PIN_TEMP3:
			GPIOB->BRR = GPIO_Pin_4;
			break;
		case PIN_TEMP4:
			GPIOB->BRR = GPIO_Pin_5;
			break;
		case PIN_RFM12B_IRQ:
			GPIOB->BRR = GPIO_Pin_6;
			break;
		case PIN_LCD_DATA1:
			GPIOC->BRR = GPIO_Pin_0;
			break;
		case PIN_LCD_DATA2:
			GPIOC->BRR = GPIO_Pin_1;
			break;
		case PIN_LCD_DATA3:
			GPIOC->BRR = GPIO_Pin_2;
			break;
		case PIN_LCD_DATA4:
			GPIOC->BRR = GPIO_Pin_3;
			break;
		case PIN_LED1:
			GPIOC->BRR = GPIO_Pin_8;
			break;
		case PIN_LED2:
			GPIOC->BRR = GPIO_Pin_9;
			break;
		case PIN_LCD_RS:
			GPIOC->BRR = GPIO_Pin_10;
			break;
		case PIN_LCD_ENABLE:
			GPIOC->BRR = GPIO_Pin_11;
			break;
		case PIN_TEMP1:
			GPIOC->BRR = GPIO_Pin_13;
			break;
	}
}

void config_port_mode (uint8_t pin, uint8_t out)
{
	switch (pin)
	{
		case PIN_TEMP2:
			GPIOB->MODER &= ~(((uint32_t)0x3) << (3*2));
			if (out)
			{
				GPIOB->MODER |= (((uint32_t)0x1) << (3*2));
			}
			break;
		case PIN_TEMP3:
			GPIOB->MODER &= ~(((uint32_t)0x3) << (4*2));
			if (out)
			{
				GPIOB->MODER |= (((uint32_t)0x1) << (4*2));
			}
			break;
		case PIN_TEMP4:
			GPIOB->MODER &= ~(((uint32_t)0x3) << (5*2));
			if (out)
			{
				GPIOB->MODER |= (((uint32_t)0x1) << (5*2));
			}
			break;
		case PIN_TEMP1:
			GPIOC->MODER &= ~(((uint32_t)0x3) << (13*2));
			if (out)
			{
				GPIOC->MODER |= (((uint32_t)0x1) << (13*2));
			}
			break;
	}
}

uint8_t config_port_read (uint8_t pin)
{
	uint8_t ret=0;
	switch (pin)
	{
		case PIN_BUTTON:
			ret = ((GPIOA->IDR & GPIO_Pin_0) == GPIO_Pin_0);
			break;
		case PIN_BTN1:
			ret = ((GPIOA->IDR & GPIO_Pin_1) == GPIO_Pin_1);
			break;
		case PIN_BTN2:
			ret = ((GPIOA->IDR & GPIO_Pin_2) == GPIO_Pin_2);
			break;
		case PIN_BTN3:
			ret = ((GPIOA->IDR & GPIO_Pin_3) == GPIO_Pin_3);
			break;
		case PIN_IRIN:
			ret = ((GPIOA->IDR & GPIO_Pin_8) == GPIO_Pin_8);
			break;
		case PIN_NEXA_IN:
			ret = ((GPIOA->IDR & GPIO_Pin_12) == GPIO_Pin_12);
			break;
		case PIN_TEMP2:
			ret = ((GPIOB->IDR & GPIO_Pin_3) == GPIO_Pin_3);
			break;
		case PIN_TEMP3:
			ret = ((GPIOB->IDR & GPIO_Pin_4) == GPIO_Pin_4);
			break;
		case PIN_TEMP4:
			ret = ((GPIOB->IDR & GPIO_Pin_5) == GPIO_Pin_5);
			break;
		case PIN_RFM12B_IRQ:
			ret = ((GPIOB->IDR & GPIO_Pin_6) == GPIO_Pin_6);
			break;
		case PIN_TEMP1:
			ret = ((GPIOC->IDR & GPIO_Pin_13) == GPIO_Pin_13);
			break;
	}
	return ret;
}
