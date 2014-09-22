#ifndef _OUT_H
#define _OUT_H

typedef void (*FnPut)(uint8_t);

extern void out_str(FnPut put, const char* str);
extern void out_byte(FnPut put, uint8_t num);
extern void out_time(FnPut put,  RTC_TimeTypeDef *time);

#endif
