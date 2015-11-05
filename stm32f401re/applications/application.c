/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include "rtuser.h"

#include "bsp_gpio.h"

#include "finsh.h"

#include "board.h"

/*@{*/
//LED task info.
THREAD_STACK_ALLOC(led_stack,256);
THREAD_TCB_ALLOC(led_thread);
extern void rt_thread_entry_led1(void* parameter);

#ifdef RT_USING_MPU6050
//dmp task info.
THREAD_STACK_ALLOC(dmp_stack,1024);
THREAD_TCB_ALLOC(dmp_thread);
extern void rt_thread_entry_dmp(void* parameter);
#endif

#ifdef RT_USING_NRF2401
//dmp task info.
THREAD_STACK_ALLOC(nrf2401_stack,512);
THREAD_TCB_ALLOC(nrf2401_thread);
extern void rt_thread_entry_2401(void* parameter);
#endif

#ifdef RT_USING_ENC28J60
//enc28j60 task info.
THREAD_STACK_ALLOC(enc28j60_stack,1024);
THREAD_TCB_ALLOC(enc28j60_thread);
extern void rt_thread_entry_enc28j60(void* parameter);
#endif

int rt_application_init()
{
		user_log(APP_INIT, "/ init those application!");
    //------- init led1 thread
    rt_thread_init(&led_thread,
                   "led1",
                   rt_thread_entry_led1,
                   RT_NULL,
                   &led_stack[0],
                   sizeof(led_stack),11,5);
    rt_thread_startup(&led_thread);
		user_log(APP_INIT, "// setup the LED Thread!\r\n");
#ifdef RT_USING_MPU6050
    //------- init dmp thread
    rt_thread_init(&dmp_thread,
                   "dmp",
                   rt_thread_entry_dmp,
                   RT_NULL,
                   &dmp_stack[0],
                   sizeof(dmp_stack),8,5);
    rt_thread_startup(&dmp_thread);
		user_log(APP_INIT, "// setup the MPU6050-DMP Thread!\r\n");
#endif
	
#ifdef RT_USING_NRF2401
    //------- init 2401 thread
    rt_thread_init(&nrf2401_thread,
                   "nrf2401",
                   rt_thread_entry_2401,
                   RT_NULL,
                   &nrf2401_stack[0],
                   sizeof(nrf2401_stack),9,5);
    rt_thread_startup(&nrf2401_thread);
		user_log(APP_INIT, "// setup the NRF24L01 Thread!\r\n");
#endif
	
#ifdef RT_USING_ENC28J60
    //------- init enc28j60 thread
    rt_thread_init(&enc28j60_thread,
                   "enc28j60",
                   rt_thread_entry_enc28j60,
                   RT_NULL,
                   &enc28j60_stack[0],
                   sizeof(enc28j60_stack),9,5);
    rt_thread_startup(&enc28j60_thread);
		user_log(APP_INIT, "// setup the ENC28j60 Thread!\r\n");
#endif

    return 0;
}

/*@}*/
