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

void button_keyDown_callback(){
		rt_kprintf("the button is keyDown!!!\n");
}

static rt_bool_t LED_TOGGLE_ON = 0;
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

#ifdef RT_USING_NUCLEOF401_RTC
void Date_print(void){
	RTC_GET();
	rt_kprintf("\r\nDate is :%d/%d/%d  %s\r\n"
	"Time is :%d-%d-%d  %d\r\n",
	RTC_YEAR(),RTC_MON(),RTC_DAY(),RTC_WEEKSTR(),
	RTC_HOUR(),RTC_MIN(),RTC_SEC(),RTC_SUB_SEC());
}
#ifdef RT_USING_FINSH
	FINSH_FUNCTION_EXPORT(Date_print,printf date and Time);
#endif

void Date_set(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec){
	if(RTC_SET(year,month,day,hour,min,sec) == 0){
		rt_kprintf("\r\nsync ok!!\r\n");
	}else{
		rt_kprintf("\r\nsync erreo!!\r\n");
	}
}
#ifdef RT_USING_FINSH
	FINSH_FUNCTION_EXPORT(Date_set,sync date and Time);
#endif
#endif


#ifdef RT_USING_NUCLEOF401_ADC_TEMP
void Temp_get(void){
		rt_kprintf("now the cpu inter-tempture is %d C\r\n",(uint16_t)(Get_Temprate()*100));
}
#ifdef RT_USING_FINSH
	FINSH_FUNCTION_EXPORT(Temp_get,sync date and Time);
#endif
#endif

#ifdef RT_USING_NUCLEOF401_FLASH

typedef struct {
	char temp_str[5*4];
	int  temp_int[2];
}flash_test_t;

flash_test_t *flash_test = (flash_test_t *)FLASH_STORAGE_START();

void Flash_writeStr(const char *str){
	if(BSPFLASH_WRITEWORDS(&(flash_test->temp_str[0]),str,sizeof(flash_test->temp_str))){
		rt_kprintf("error write Flash!!\r\n");
	}else{
		rt_kprintf("ok write Flash!!\r\n");
	}
}
#ifdef RT_USING_FINSH
	FINSH_FUNCTION_EXPORT(Flash_writeStr,write a max str[20] toflash );
#endif

void Flash_readCH(){
		rt_kprintf("read from Flash %c %c !!\r\n",
			BSPFLASH_READWORD(&(flash_test->temp_str[0])),BSPFLASH_READWORD(&(flash_test->temp_str[2])));
}
#ifdef RT_USING_FINSH
	FINSH_FUNCTION_EXPORT(Flash_readCH,read str[0] str[2] for you);
#endif

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
