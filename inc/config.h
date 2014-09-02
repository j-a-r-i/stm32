#ifndef _CONFIG_H
#define _CONFIG_H

extern void config_port_init (void);
#define set_LCD_DATA1 GPIOC->BSRR = GPIO_Pin_0
#define clr_LCD_DATA1 GPIOC->BRR  = GPIO_Pin_0
#define set_LCD_DATA2 GPIOC->BSRR = GPIO_Pin_1
#define clr_LCD_DATA2 GPIOC->BRR  = GPIO_Pin_1
#define set_LCD_DATA3 GPIOC->BSRR = GPIO_Pin_2
#define clr_LCD_DATA3 GPIOC->BRR  = GPIO_Pin_2
#define set_LCD_DATA4 GPIOC->BSRR = GPIO_Pin_3
#define clr_LCD_DATA4 GPIOC->BRR  = GPIO_Pin_3
#define set_LED1 GPIOC->BSRR = GPIO_Pin_8
#define clr_LED1 GPIOC->BRR  = GPIO_Pin_8
#define set_LED2 GPIOC->BSRR = GPIO_Pin_9
#define clr_LED2 GPIOC->BRR  = GPIO_Pin_9
#define set_LCD_RS GPIOC->BSRR = GPIO_Pin_10
#define clr_LCD_RS GPIOC->BRR  = GPIO_Pin_10
#define set_LCD_ENABLE GPIOC->BSRR = GPIO_Pin_11
#define clr_LCD_ENABLE GPIOC->BRR  = GPIO_Pin_11

#endif
