#include "ds1820.h"


void ds1820_init()
{
}

void ds1820_reset()
{
	// do reset

	// do presense
}

void ds1820_write(uint8_t data)
{
}

uint8_t ds1820_read()
{
}

// based on DS18S20 OPERATION EXAMPLE 3
//
void ds1820_read_temp()
{
	uint8_t temp_lsb;
	uint8_t temp_msb;
	uint8_t foo;
	uint8_t count_remain;
	uint8_t count_per_c;

	ds1820_reset();
	ds1820_write(DS1820_CMD_SKIP_ROM);
	ds1820_write(DS1820_CMD_CONVERT);
	delay();
	ds1820_reset();
	ds1820_write(DS1820_CMD_SKIP_ROM);
	ds1820_write(DS1820_CMD_READ_SCRATCHPAD);

	// Read scratchpad
	//
	temp_lsb = ds1820_read();
	temp_msb = ds1820_read();
	ds1820_read(); // Th register or user byte 1
	ds1820_read(); // Tl register or user byte 2
	ds1820_read(); // reserved
	ds1820_read(); // reserved
	count_remain = ds1820_read();
	count_per_c  = ds1820_read();
	ds1820_read(); // CRC

	ds1820_reset();
}

