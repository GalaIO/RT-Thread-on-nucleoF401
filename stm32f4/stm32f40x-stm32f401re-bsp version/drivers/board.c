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

#include <rthw.h>
#include <rtthread.h>

#include "stm32f4xx.h"
#include "board.h"

/**
 * @addtogroup STM32
 */

/*@{*/

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
extern void nucleo_borad_init(void);
void rt_hw_board_init()
{
	/* NVIC Configuration */
	NVIC_Configuration();

	rt_hw_usart_init();
#ifdef RT_USING_CONSOLE
	rt_console_set_device(CONSOLE_DEVICE);
#endif

	/* Configure the SysTick */
	SysTick_Configuration();
	
	nucleo_borad_init();
}

/*@}*/
