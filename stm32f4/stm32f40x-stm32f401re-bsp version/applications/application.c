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

#include "bsp_exti.h"

//User LED on nucleo borad.
#define LED_INIT()	{\
RCC_CMD(PA_PER);\
PAOUT_INT(BIT5);\
}
#define LED_ON()		PAOUT_SET(BIT5)
#define LED_OFF()		PAOUT_CLR(BIT5)

//User key on nucleo borad.
#define BUTTON_INIT()	{\
RCC_CMD(PC_PER);\
PCIN_INT(BIT13);\
}
#define BUTTON_STATUS()	PCIN_GET(BIT13)

void nucleo_borad_init(){
		LED_INIT();
		BUTTON_INIT();			//init gpio as input.
		
		SYSCFG_ENABLE();		//cmd exti bus as syscfg.
		PCEXTI_INT(PIN13,EXTI_F);	//init exti for pc13.
		NVIC_CMD(EXTI15_10_IRQn,2,2);	//init nvic priory for EXTI15_10_IRQn.

}

void button_keyDown_callback(){
		rt_kprintf("the button is keyDown!!!\n");
}

static rt_bool_t LED_TOGGLE_ON =1;
THREAD_STACK_ALLOC(led_stack,256);
THREAD_TCB_ALLOC(led_thread);
static void rt_thread_entry_led1(void* parameter)
{
    while (1)
    {
			if(LED_TOGGLE_ON){
        LED_ON();

        /* Insert delay */
        rt_thread_delay(RT_TICK_PER_SECOND/2);

        LED_OFF();

        /* Insert delay */
        rt_thread_delay(RT_TICK_PER_SECOND/2);
			}else{
				rt_thread_delay(RT_TICK_PER_SECOND/2);
			}

    }
}

int led_green(rt_bool_t status){
		if(status){
			LED_ON();
		}else{
			LED_OFF();
		}
		return 0;
}
#ifdef RT_USING_FINSH
	FINSH_FUNCTION_EXPORT(led_green,set the green led status);
#endif


int led_toggle_ch(rt_bool_t status){
		if(status){
			LED_TOGGLE_ON = 1;
		}else{
			LED_TOGGLE_ON = 0;
		}
		return 0;
}
#ifdef RT_USING_FINSH
	FINSH_FUNCTION_EXPORT(led_toggle_ch,toggle the green led way);
#endif

int button_status(void){
		if(!BUTTON_STATUS()) 	rt_kprintf("button is pressed!!!\n");
		else 	rt_kprintf("button is realsed!!!\n");
		return 0;
}
#ifdef RT_USING_FINSH
	FINSH_FUNCTION_EXPORT(button_status,query button status);
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
    rt_thread_startup(&led_thread);

    return 0;
}

/*@}*/
