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

/*----------------------------------------------------Head Resource------------------------------------------------------------*/
//f4 ��Դ
#include "stm32f4xx.h"
//������Դ
#include <board.h>
/* ��ΪҪʹ��RT-Thread���̷߳�����Ҫ����RT-Thread��ͷ�ļ� */
#include <rtthread.h>


/*----------------------------------------------------Thread Interface------------------------------------------------------------*/
/* �߳��õ���ջ������ARMΪ4�ֽڶ��룬����ջ�ռ������4�ֽڱ��� 
	����ʹ�����к� ����֤�����ռ�ռ���4�ֽڶ���� ʹ��RT-Thread�ĺ�*/
#define THREAD_STACK_ALLOC(name,size)			char name[RT_ALIGN(size,RT_ALIGN_SIZE)]
/* �̵߳�TCB���ƿ� */
#define THREAD_TCB_ALLOC(name)						struct rt_thread name
#define THREAD_STACK_EXTERN(name,size)		extern char name[RT_ALIGN(size,RT_ALIGN_SIZE)]
#define THREAD_TCB_EXTERN(name)						extern struct rt_thread name
/* ����һ��ʹ�� rt_thread_create������һ����̬��������ú�������ֱ������ջ�ռ� ���Ա����ȼ�飬Ȼ������*/
#define THREAD_CHECK_STARTUP(thread)			if(thread!=RT_NULL)	rt_thread_startup(thread)
/* ֱ�������ȫ�ֱ�����ʹ�� rt_thread_init��ʼ�����ɣ�Ȼ������*/
#define THREAD_STARTUP(thread)						rt_thread_startup(thread)


/*----------------------------------------------------Debugger------------------------------------------------------------*/
#define user_log(logger,format,...)				if(logger) rt_kprintf("[USER]["#logger"][Log]: "format"\r\n", ##__VA_ARGS__)
#define user_err(logger,format,...)				if(logger) rt_kprintf("[USER]["#logger"][Error][%s:%4d]: "format"\r\n", SHORT_FILE, __LINE__, ##__VA_ARGS__)
#define user_waring(logger,format,...)		if(logger) rt_kprintf("[USER]["#logger"][Waring]: "format"\r\n", ##__VA_ARGS__)

#define NUCLEO_BOARD			1

#define APP_INIT					1



#endif
