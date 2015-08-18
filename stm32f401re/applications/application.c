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
THREAD_STACK_ALLOC(led_stack,512);
THREAD_TCB_ALLOC(led_thread);
extern void rt_thread_entry_led1(void* parameter);

#ifdef RT_USING_MPU6050
//dmp task info.
THREAD_STACK_ALLOC(dmp_stack,1024);
THREAD_TCB_ALLOC(dmp_thread);
extern void rt_thread_entry_dmp(void* parameter);
#endif
int rt_application_init()
{
    //------- init led1 thread
    rt_thread_init(&led_thread,
                   "led1",
                   rt_thread_entry_led1,
                   RT_NULL,
                   &led_stack[0],
                   sizeof(led_stack),11,5);
#ifdef RT_USING_MPU6050
    //------- init dmp thread
    rt_thread_init(&dmp_thread,
                   "dmp",
                   rt_thread_entry_dmp,
                   RT_NULL,
                   &dmp_stack[0],
                   sizeof(dmp_stack),8,5);
    rt_thread_startup(&dmp_thread);
#endif
    rt_thread_startup(&led_thread);

    return 0;
}

/*@}*/
