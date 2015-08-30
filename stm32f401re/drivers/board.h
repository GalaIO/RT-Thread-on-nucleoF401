/*
 * File      : board.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-09-22     Bernard      add board.h to this bsp
 */
/*
 *	Author:		GalaIO
 *	Date:			2015-7-21 8:46 PM
 *	Description:
 *		Arduino interface:
 *
 *																													D15		PB8  -10-	I2C1_SCL//,TIM4_CH3
 *																													D14		PB9	 -9-	I2C1_SDA//,TIM4_CH4
 *																						   								 AVDD  -8-
 *			-1-  NC																	 								  GND	 -7-
 *			-2-  IOREF																					D13	 PB13	 -6-  User_LED//
 *			-3-  RESET																					D12		PA6  -5-	SPI1_MISO//,TIM3_CH1,ADC1_IN6
 *			-4-  +3V3																						D11		PA7  -4-	SPI1_MOSI//,TIM1_CH1N,TIM3_CH2,ADC1_IN7
 *			-5-  +5V																						D10		PB6  -3-	SPI1_CS//,TIM4_CH1
 *			-6-  GND																						D9		PC7	 -2-	//TIM3_CH2
 *			-7-  GND																						D8		PA9	 -1-	USART1_TX//,TIM1_CH2
 *			-8-  VIN
 *																													D7		PA8  -8-	//TIM1_CH1
 *			-1-  PA0	A0	TIM2_CH1//,TM5_CH1,ADC1_IN0,WKUP			D6	 PB10	 -7-	SPI3_CS|TIM2_CH3
 *			-2-  PA1	A1	TIM2_CH2//,TIM5_CH2,ADC1_IN1					D5		PB4  -6-	SPI3_MISO|TIM3_CH1
 *			-3-	 PA4	A2	ADC1_IN4//														D4		PB5  -5-	SPI3_MOSI|TIM3_CH2
 *			-4-  PB0	A3	TIM3_CH3//,TIM1_CH2N,ADC1_IN8					D3		PB3  -4-	SPI3_SCK//,TIM2_CH2
 *			-5-  PC1	A4	ADC1_IN11//														D2	 PA10  -3-	USART1_RX//,TIM1_CH3
 *      -6-	 PC0	A5	ADC1_IN10//														D1		PA2	 -2-  USART2_TX//,TIM2_CH3,TIM5_CH3,ADC1_IN2
 *																													D0	  PA3  -1-  USART2_RX//,TIM2_CH4,TIM5_CH4,ADC1_IN3
 *		Note:需要注意的是对于SPI1，PA5-SPI1_SCK//,TIM2_CH1,ADC1_IN5，不在上面。
 *					PC13-User_button。
**/
// <<< Use Configuration Wizard in Context Menu >>>
#ifndef __BOARD_H__
#define __BOARD_H__

#include <stm32f4xx.h>

#include <rthw.h>
#include <rtthread.h>
#include "bsp_exti.h"

#include "bsp_gpio.h"

#include "bsp_timer.h"

#include "bsp_rtc.h"

#include "bsp_adc.h"

#include "bsp_flash.h"

// <o> Internal SRAM memory size[Kbytes] <8-64>
//	<i>Default: 96
#define STM32_SRAM_SIZE         96     //stm32f401re
#define STM32_SRAM_END          (0x20000000 + STM32_SRAM_SIZE * 1024)


void rt_hw_board_init(void);

#if STM32_CONSOLE_USART == 0
#define CONSOLE_DEVICE "no"
#elif STM32_CONSOLE_USART == 1
#define CONSOLE_DEVICE "uart1"
#elif STM32_CONSOLE_USART == 2
#define CONSOLE_DEVICE "uart2"
#elif STM32_CONSOLE_USART == 3
#define CONSOLE_DEVICE "uart3"
#endif

#define FINSH_DEVICE_NAME   CONSOLE_DEVICE


//init user led on board.
#ifdef RT_USING_NUCLEOF401_USERLED

//User LED on nucleo borad.
#define LED_INIT()	{\
RCC_CMD(PB_PER);\
PBOUT_INT(BIT13);\
}
#define LED_ON()		PBOUT_SET(BIT13)
#define LED_OFF()		PBOUT_CLR(BIT13)
#define LED_TOG()		PBOUT_TOG(BIT13)

#else

#define LED_INIT()	
#define LED_ON()		
#define LED_OFF()		
#define LED_TOG()		

#endif

//init user button on board.
#ifdef RT_USING_NUCLEOF401_USERBUTTON
//User key on nucleo borad.
//initialization steps.
//1.cmd pc bus.
//2.init pc_13 as input io.
//3.cmd exti bus as syscfg.
//4.init exti for pc13.
//5.init nvic priory for EXTI15_10_IRQn.
#define BUTTON_INIT()	{\
RCC_CMD(PC_PER);\
PCIN_INT(BIT13);\
SYSCFG_ENABLE();\
PCEXTI_INT(PIN13,EXTI_F);\
GENERAL_NVIC_CMD(EXTI15_10_IRQn);\
}
#define BUTTON_STATUS()	PCIN_GET(BIT13)

#else
//User key on nucleo borad.
#define BUTTON_INIT()	
#define BUTTON_STATUS()	

#endif

void rt_hw_usart_init(void);

/* SD Card init function */
void rt_hw_msd_init(void);

#endif
