#ifndef __USART_H
#define __USART_H

extern void usart_init(void);
extern void usart_put(uint8_t ch);
extern void usart_str(const char *s);

#endif
