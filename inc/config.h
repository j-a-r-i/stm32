#ifndef _CONFIG_H
#define _CONFIG_H

extern void config_port_init (void);
extern void config_port_set (uint8_t pin);
extern void config_port_clear (uint8_t pin);
extern void config_port_mode (uint8_t pin, uint8_t out);
extern uint8_t config_port_read (uint8_t pin);
#define set_TEMP1 GPIOB->BSRR = GPIO_Pin_3
#define clr_TEMP1 GPIOB->BRR  = GPIO_Pin_3
#define toggle_TEMP1 GPIOB->ODR ^= GPIO_Pin_3
#define set_TEMP2 GPIOB->BSRR = GPIO_Pin_4
#define clr_TEMP2 GPIOB->BRR  = GPIO_Pin_4
#define toggle_TEMP2 GPIOB->ODR ^= GPIO_Pin_4
#define set_TEMP3 GPIOB->BSRR = GPIO_Pin_5
#define clr_TEMP3 GPIOB->BRR  = GPIO_Pin_5
#define toggle_TEMP3 GPIOB->ODR ^= GPIO_Pin_5
#define set_LCD_DATA1 GPIOC->BSRR = GPIO_Pin_0
#define clr_LCD_DATA1 GPIOC->BRR  = GPIO_Pin_0
#define toggle_LCD_DATA1 GPIOC->ODR ^= GPIO_Pin_0
#define set_LCD_DATA2 GPIOC->BSRR = GPIO_Pin_1
#define clr_LCD_DATA2 GPIOC->BRR  = GPIO_Pin_1
#define toggle_LCD_DATA2 GPIOC->ODR ^= GPIO_Pin_1
#define set_LCD_DATA3 GPIOC->BSRR = GPIO_Pin_2
#define clr_LCD_DATA3 GPIOC->BRR  = GPIO_Pin_2
#define toggle_LCD_DATA3 GPIOC->ODR ^= GPIO_Pin_2
#define set_LCD_DATA4 GPIOC->BSRR = GPIO_Pin_3
#define clr_LCD_DATA4 GPIOC->BRR  = GPIO_Pin_3
#define toggle_LCD_DATA4 GPIOC->ODR ^= GPIO_Pin_3
#define set_LED1 GPIOC->BSRR = GPIO_Pin_8
#define clr_LED1 GPIOC->BRR  = GPIO_Pin_8
#define toggle_LED1 GPIOC->ODR ^= GPIO_Pin_8
#define set_LED2 GPIOC->BSRR = GPIO_Pin_9
#define clr_LED2 GPIOC->BRR  = GPIO_Pin_9
#define toggle_LED2 GPIOC->ODR ^= GPIO_Pin_9
#define set_LCD_RS GPIOC->BSRR = GPIO_Pin_10
#define clr_LCD_RS GPIOC->BRR  = GPIO_Pin_10
#define toggle_LCD_RS GPIOC->ODR ^= GPIO_Pin_10
#define set_LCD_ENABLE GPIOC->BSRR = GPIO_Pin_11
#define clr_LCD_ENABLE GPIOC->BRR  = GPIO_Pin_11
#define toggle_LCD_ENABLE GPIOC->ODR ^= GPIO_Pin_11
#define PIN_BUTTON 1
#define PIN_TEMP1 2
#define PIN_TEMP2 3
#define PIN_TEMP3 4
#define PIN_LCD_DATA1 5
#define PIN_LCD_DATA2 6
#define PIN_LCD_DATA3 7
#define PIN_LCD_DATA4 8
#define PIN_LED1 9
#define PIN_LED2 10
#define PIN_LCD_RS 11
#define PIN_LCD_ENABLE 12

#endif
