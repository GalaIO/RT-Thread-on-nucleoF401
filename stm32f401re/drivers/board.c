/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      first implementation
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
 *			-1-  NC																	 								 GND	 -7-
 *			-2-  IOREF																					D13		PA5	 -6-  SPI1_SCK//,TIM2_CH1,ADC1_IN5
 *			-3-  RESET																					D12		PA6  -5-	SPI1_MISO//,TIM3_CH1,ADC1_IN6
 *			-4-  +3V3																						D11		PA7  -4-	SPI1_MOSI//,TIM1_CH1N,TIM3_CH2,ADC1_IN7
 *			-5-  +5V																						D10		PB6  -3-	SPI1_CS//,TIM4_CH1
 *			-6-  GND																						D9		PC7	 -2-	TIM3_CH2//
 *			-7-  GND																						D8		PA9	 -1-	USART1_TX//,TIM1_CH2
 *			-8-  VIN
 *																													D7		PA8  -8-	//TIM1_CH1
 *			-1-  PA0	A0	TIM2_CH1//,TM5_CH1,ADC1_IN0,WKUP			D6	 PB10	 -7-	SPI3_CS|TIM2_CH3
 *			-2-  PA1	A1	TIM2_CH2//,TIM5_CH2,ADC1_IN1					D5		PB4  -6-	SPI3_MISO|TIM3_CH1
 *			-3-	 PA4	A2	ADC1_IN4//														D4		PB5  -5-	SPI3_MOSI|TIM3_CH2
 *			-4-  PB0	A3	TIM3_CH3//,TIM1_CH2N,ADC1_IN8					D3		PB3  -4-	SPI3_SCK|TIM2_CH2
 *			-5-  PC1	A4	ADC1_IN11//														D2	 PA10  -3-	USART1_RX//,TIM1_CH3
 *      -6-	 PC0	A5	ADC1_IN10//														D1		PA2	 -2-  USART2_TX//,TIM2_CH3,TIM5_CH3,ADC1_IN2
 *																													D0	  PA3  -1-  USART2_RX//,TIM2_CH4,TIM5_CH4,ADC1_IN3
 *
**/


#include "stm32f4xx.h"
#include "board.h"

/**
 * @addtogroup STM32
 */

#include "rtc.h"
void nucleo_borad_init(){
		
#ifdef RT_USING_NUCLEOF401_RTC
		//start RTC.
		RTC_START();
#ifdef RTC_DEVICE_USING_BLOCK
		rtc_device_register("rtc");
#endif /*RTC_DEVICE_USING_BLOCK*/

#ifdef RTC_DEVICE_USING_RTC_STRUCT
		rdt_device_register("rdt");
#endif /*RTC_DEVICE_USING_RTC_STRUCT*/
	
#endif
	
		LED_INIT();
		BUTTON_INIT();		

#ifdef RT_USING_NUCLEOF401_TIM2
		//init TIM2 update.
		TIM2_START_US(1000);
	
#ifdef RT_USING_NUCLEOF401_TIM2_PWM	
		//init PA0 PWM
		TIM2_PWM_PA0(500);
		TIM2_PWM_PA1(500);
#endif
	
#ifdef RT_USING_NUCLEOF401_ADC_TEMP
	ADC_INERTEMP_INIT();
#endif
	
#endif
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

/*******************************************************************************
 * Function Name  : SysTick_Configuration
 * Description    : Configures the SysTick for OS tick.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void  SysTick_Configuration(void)
{
	RCC_ClocksTypeDef  rcc_clocks;
	rt_uint32_t         cnts;

	RCC_GetClocksFreq(&rcc_clocks);

	cnts = (rt_uint32_t)rcc_clocks.HCLK_Frequency / RT_TICK_PER_SECOND;
	rt_kprintf("\r\n-------*****************---------\r\n"
		"    the system clock: %u Hz\r\n"
		"    the  AHB   clock: %u Hz\r\n"
		"    the  APB1  clock: %u Hz\r\n"
		"    the  APB2  clock: %u Hz\r\n"
	  "-------*****************---------\r\n",
		(rt_uint32_t)rcc_clocks.SYSCLK_Frequency,
		(rt_uint32_t)rcc_clocks.HCLK_Frequency,
		(rt_uint32_t)rcc_clocks.PCLK1_Frequency,
		(rt_uint32_t)rcc_clocks.PCLK2_Frequency);
	//check the macro configuration correct?
	if((rt_uint32_t)rcc_clocks.SYSCLK_Frequency != SYSCLK_HZ 
			&& (rt_uint32_t)rcc_clocks.PCLK1_Frequency != PCLK1_HZ
			&& (rt_uint32_t)rcc_clocks.PCLK2_Frequency != PCLK2_HZ){
			rt_kprintf("That's horrible ! Your configuration is wrong ,find it and trap it !!\r\n");
			while(1);	
	}
	cnts = cnts / 8;

	SysTick_Config(cnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	rt_tick_increase();

	/* leave interrupt */
	rt_interrupt_leave();
}

/**
 * This function will initial STM32 board.
 */
void rt_hw_board_init()
{
	/* NVIC Configuration */
	NVIC_Configuration();

	rt_hw_usart_init();
#ifdef RT_USING_CONSOLE
	rt_console_set_device(CONSOLE_DEVICE);
#endif

	nucleo_borad_init();
	
	/* Configure the SysTick */
	SysTick_Configuration();
	
}

/*@}*/
