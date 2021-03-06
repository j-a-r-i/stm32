#include "hw.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_misc.h"
#include "queue.h"
#include "test.h"

#define FIFO_SIZE 40
static queue_t     txfifo;
static uint8_t     txfifoData[FIFO_SIZE];

//------------------------------------------------------------------------------
void usart_init(void)
{
	USART_InitTypeDef initUsart;
	USART_ClockInitTypeDef clockInit;
	NVIC_InitTypeDef nvicInit;

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

    //NVIC_EnableIRQ(USART1_IRQn);

	nvicInit.NVIC_IRQChannel = USART1_IRQn;
	nvicInit.NVIC_IRQChannelPriority = 0x02;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicInit);

	USART_Cmd(USART1, ENABLE);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_TXE,  ENABLE);

	queue_init(&txfifo, FIFO_SIZE, txfifoData);	
}

//------------------------------------------------------------------------------
void usart_put(uint8_t ch)
{
	  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, (uint8_t) ch);
	//queue_push(&txfifo, ch);
}

//------------------------------------------------------------------------------
void usart_str(const char *s)
{
	while(*s) {
		while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
		USART_SendData(USART1, *s);
		//queue_push(&txfifo, *s);
		s++;
	}
	//if (queue_some(&txfifo) && (USART_GetFlagStatus(USART1, USART_FLAG_TC) == SET))
	USART_SendData(USART1, queue_pop(&txfifo));
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
void USART1_IRQHandler(void) 
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) { // Received characters modify string
		uint8_t ch = USART1->RDR;
		test_exec(ch);
	}
	/*if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) {
		if (queue_some(&txfifo))
			USART_SendData(USART1, queue_pop(&txfifo));
	}*/
}
