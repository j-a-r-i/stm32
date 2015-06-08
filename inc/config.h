#ifndef _CONFIG_H
#define _CONFIG_H

extern void io_init (void);
extern void io_set (uint8_t pin);
extern void io_clear (uint8_t pin);
extern void io_mode (uint8_t pin, uint8_t out);
extern uint8_t io_read (uint8_t pin);
#define set_NEXA_OUT GPIOA->BSRR = GPIO_Pin_11
#define clr_NEXA_OUT GPIOA->BRR  = GPIO_Pin_11
#define toggle_NEXA_OUT GPIOA->ODR ^= GPIO_Pin_11
#define set_IROUT GPIOA->BSRR = GPIO_Pin_13
#define clr_IROUT GPIOA->BRR  = GPIO_Pin_13
#define toggle_IROUT GPIOA->ODR ^= GPIO_Pin_13
#define set_CS_RFM12B GPIOB->BSRR = GPIO_Pin_0
#define clr_CS_RFM12B GPIOB->BRR  = GPIO_Pin_0
#define toggle_CS_RFM12B GPIOB->ODR ^= GPIO_Pin_0
#define set_CS_W5200 GPIOB->BSRR = GPIO_Pin_1
#define clr_CS_W5200 GPIOB->BRR  = GPIO_Pin_1
#define toggle_CS_W5200 GPIOB->ODR ^= GPIO_Pin_1
#define set_CS_SDCARD GPIOB->BSRR = GPIO_Pin_2
#define clr_CS_SDCARD GPIOB->BRR  = GPIO_Pin_2
#define toggle_CS_SDCARD GPIOB->ODR ^= GPIO_Pin_2
#define set_TEMP2 GPIOB->BSRR = GPIO_Pin_3
#define clr_TEMP2 GPIOB->BRR  = GPIO_Pin_3
#define toggle_TEMP2 GPIOB->ODR ^= GPIO_Pin_3
#define set_TEMP3 GPIOB->BSRR = GPIO_Pin_4
#define clr_TEMP3 GPIOB->BRR  = GPIO_Pin_4
#define toggle_TEMP3 GPIOB->ODR ^= GPIO_Pin_4
#define set_TEMP4 GPIOB->BSRR = GPIO_Pin_5
#define clr_TEMP4 GPIOB->BRR  = GPIO_Pin_5
#define toggle_TEMP4 GPIOB->ODR ^= GPIO_Pin_5
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
#define set_TEMP1 GPIOC->BSRR = GPIO_Pin_13
#define clr_TEMP1 GPIOC->BRR  = GPIO_Pin_13
#define toggle_TEMP1 GPIOC->ODR ^= GPIO_Pin_13
#define PIN_BUTTON 1
#define PIN_BTN1 2
#define PIN_BTN2 3
#define PIN_BTN3 4
#define PIN_IRIN 5
#define PIN_NEXA_OUT 6
#define PIN_NEXA_IN 7
#define PIN_IROUT 8
#define PIN_CS_RFM12B 9
#define PIN_CS_W5200 10
#define PIN_CS_SDCARD 11
#define PIN_TEMP2 12
#define PIN_TEMP3 13
#define PIN_TEMP4 14
#define PIN_RFM12B_IRQ 15
#define PIN_LCD_DATA1 16
#define PIN_LCD_DATA2 17
#define PIN_LCD_DATA3 18
#define PIN_LCD_DATA4 19
#define PIN_LED1 20
#define PIN_LED2 21
#define PIN_LCD_RS 22
#define PIN_LCD_ENABLE 23
#define PIN_TEMP1 24

#endif
