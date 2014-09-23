#include "stm32f0xx.h"
#include "w5200.h"

//------------------------------------------------------------------------------
void w5200_init(void)
{
	SPI_InitTypeDef spiInit;

	// do SPI initialization
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	spiInit.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spiInit.SPI_Mode      = SPI_Mode_Master;
	spiInit.SPI_DataSize  = SPI_DataSize_8b;
	spiInit.SPI_CPOL      = SPI_CPOL_Low;
	spiInit.SPI_CPHA      = SPI_CPHA_1Edge;
	spiInit.SPI_NSS       = SPI_NSS_Hard;
	spiInit.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	spiInit.SPI_FirstBit  = SPI_FirstBit_MSB;
	   
	SPI_Init(SPI1, &spiInit);

	SPI_Cmd(SPI1, ENABLE);
}

//------------------------------------------------------------------------------
void w5200_write(uint16_t reg, uint8_t value)
{
	SPI_SendData8(SPI1, reg >> 8);
	SPI_SendData8(SPI1, reg & 0xFF);
	SPI_SendData8(SPI1, 0x80);
	SPI_SendData8(SPI1, 0x01);
	SPI_SendData8(SPI1, value);
}

//------------------------------------------------------------------------------
uint8_t w5200_read(uint16_t reg)
{
	SPI_SendData8(SPI1, reg >> 8);
	SPI_SendData8(SPI1, reg & 0xFF);
	SPI_SendData8(SPI1, 0x00);
	SPI_SendData8(SPI1, 0x01);
	return SPI_ReceiveData8(SPI1);
}

