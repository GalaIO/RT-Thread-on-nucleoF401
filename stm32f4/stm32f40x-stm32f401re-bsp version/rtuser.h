/*
 *
 *Author:				GalaIO
 *Date:					2015-7-10 3:16 PM
 *Description:	-build a user tool for rt-thread.
 *							-fix a bug.
 *							 -remove options->c/c++->define->STM32F10X_HD
 *							 -keil will define a macro STM32F10X_HD | STM32F10X_MD | STM32F10X_LD | STM32F10X_CL 
 *							 autoly by device whitch you selected.
 *
 *
 *Author:				GalaIO
 *Date:					2015-7-15 8:13 PM
 *Description:	-add THREAD_CHECK_STARTUP and THREAD_STARTUP for user tools.
 *							-fix THREAD_STACK_ALLOC using a RT_ALIEN to alien 4 byte for ARM.
 *
 *
**/

#ifndef _RTUSER_H_
#define _RTUSER_H_

//f4 资源
#include "stm32f4xx.h"
//板载资源
#include <board.h>
/* 因为要使用RT-Thread的线程服务，需要包含RT-Thread的头文件 */
#include <rtthread.h>
/* 线程用到的栈，由于ARM为4字节对齐，所以栈空间必须是4字节倍数 
	可以使用下列宏 来保证申请的占空间是4字节对齐的 使用RT-Thread的宏*/
#define THREAD_STACK_ALLOC(name,size)			char name[RT_ALIGN(size,RT_ALIGN_SIZE)]
/* 线程的TCB控制块 */
#define THREAD_TCB_ALLOC(name)						struct rt_thread name
/* 启动一个使用 rt_thread_create创建的一个动态对象，另外该函数可以直接申请栈空间 所以必须先检查，然后启动*/
#define THREAD_CHECK_STARTUP(thread)			if(thread!=RT_NULL)	rt_thread_startup(thread)
/* 直接申请的全局变量，使用 rt_thread_init初始化即可，然后启动*/
#define THREAD_STARTUP(thread)						rt_thread_startup(thread)

#endif
