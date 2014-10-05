/** STM32 Standard peripheral library needs this header.
 */
#ifndef _STM32F0XX_CONF_H
#define _STM32F0XX_CONF_H

// include all needed std peripheral modules here.
//
#include "stm32f0xx_exti.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_syscfg.h"
#include "stm32f0xx_pwr.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_rtc.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_misc.h"

#define assert_param(expr) ((void)0)

#endif
