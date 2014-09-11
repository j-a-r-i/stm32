#ifndef __DS18B20_H
#define __DS18B20_H

//
// http://datasheets.maximintegrated.com/en/ds/DS18S20.pdf
//

#define DS1820_CMD_CONVERT           0x44
#define DS1820_CMD_WRITE_SCRATCHPAD  0x4E
#define DS1820_CMD_READ_SCRATCHPAD   0xBE
#define DS1820_CMD_SKIP_ROM          0xCC

extern void ds1820_init();
extern void ds1820_read_temp();

#endif