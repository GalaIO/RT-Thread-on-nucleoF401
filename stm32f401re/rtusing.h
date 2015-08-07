/*
 *******************************************************************
 *							RT_USING
 *******************************************************************
 *
 *Author:				GalaIO
 *Date:					2015-7-15 8:13 PM
 *Description:	-add THREAD_CHECK_STARTUP and THREAD_STARTUP for user tools.
 *							-fix THREAD_STACK_ALLOC using a RT_ALIEN to alien 4 byte for ARM.
 *
 * *
 *Author:				GalaIO
 *Date:					2015-7-15 8:13 PM
 *Description:	-add a new macro for init the sepcial board and resources on it.
 *							
 *
 *
**/
#ifndef _RTUSING_H_
#define _RTUSING_H_

///
///
////target board.
///
///
#ifndef RT_USING_NUCLEOF401

#define RT_USING_NUCLEOF401
//
/////resources on board.
//
#define RT_USING_NUCLEOF401_USERLED
#define RT_USING_NUCLEOF401_USERBUTTON

//RTC  Real-Time-Clock
#define RT_USING_NUCLEOF401_RTC
//#define RT_USING_NUCLEOF401_TIM2
#define RT_USING_NUCLEOF401_TIM2_PWM
//#define RT_USING_NUCLEOF401_TIM3
#define RT_USING_NUCLEOF401_TIM3_PWM
//using ADC and inter-TEMP
<<<<<<< HEAD
//#define RT_USING_NUCLEOF401_ADC_TEMP
//#define RT_USING_NUCLEOF401_FLASH

#define RT_USING_IIC1
#define RT_USING_MPU6050
#define RT_USING_DMP
=======
#define RT_USING_NUCLEOF401_ADC_TEMP
#define RT_USING_NUCLEOF401_FLASH

#define RT_USING_IIC1
#define RT_USING_MPU6050

>>>>>>> 2fb89b191734984412ab9b61950f442911b42e58
#endif


#ifndef RT_USING_TIMER_SOFT
/*
 *open RT_USING_TIMER_SOFT;
*/
//#define RT_USING_TIMER_SOFT
#endif


#ifndef RT_USING_HOOK
/*
 *open RT_USING_HOOK;
*/
//#define RT_USING_HOOK
#endif


#ifndef RT_USING_HEAP				 //open by default.
//we just using heap and small mem , not a slab.
/* Using Dynamic Heap Management and using it for xxx_create*/
 #define RT_USING_HEAP

/* Using Small MM */
#define RT_USING_SMALL_MEM
#endif


#ifndef RT_USING_MODULE
/*
 *open RT_USING_MODULE;
*/
//#define RT_USING_MODULE
#endif


#ifndef RT_USING_OVERFLOW_CHECK   //open by default.
/*
 *open RT_USING_OVERFLOW_CHECK;
*/
#define RT_USING_OVERFLOW_CHECK
#endif


#ifndef RT_USING_SEMAPHORE			 //open by default.
/*
 *open RT_USING_SEMAPHORE;
*/
#define RT_USING_SEMAPHORE
#endif


#ifndef RT_USING_MUTEX					 //open by default.
/*
 *open RT_USING_MUTEX;
*/
#define RT_USING_MUTEX
#endif


#ifndef RT_USING_EVENT
/*
 *open RT_USING_EVENT;
*/
//#define RT_USING_EVENT
#endif


#ifndef RT_USING_MAILBOX				 //open by default.
/*
 *open RT_USING_MAILBOX;
*/
#define RT_USING_MAILBOX
#endif


#ifndef RT_USING_MESSAGEQUEUE
/*
 *open RT_USING_MESSAGEQUEUE;
*/
//#define RT_USING_MESSAGEQUEUE
#endif


#ifndef RT_USING_MEMPOOL				 //open by default.
/*
 *open RT_USING_MEMPOOL;
*/
#define RT_USING_MEMPOOL
#endif


#ifndef RT_USING_DEVICE				 //open by default.
/*
 *open RT_USING_DEVICE;
*/
#define RT_USING_DEVICE
#endif


#ifndef RT_USING_FINSH
/* SECTION: finsh, a C-Express shell */
#define RT_USING_FINSH
/* Using symbol table */
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
#endif


#ifndef RT_USING_CONSOLE
/* SECTION: Console options */
#define RT_USING_CONSOLE
/* the buffer size of console*/
#define RT_CONSOLEBUF_SIZE	512  //more room more better.
// <o> Console on USART: <0=> no console <1=>USART 1 <2=>USART 2 <3=> USART 3
// 	<i>Default: 1
#define STM32_CONSOLE_USART		2			//nucleo connected usart2 for debug.
#endif


#ifndef RT_USING_UART1				 //open by default.
/*
 *open RT_USING_UART1;
*/
#define RT_USING_UART1
#endif


#ifndef RT_USING_UART2
/*
 *open RT_USING_UART2;
*/
#define RT_USING_UART2
#endif


#ifndef RT_USING_UART3
/*
 *open RT_USING_UART3;
*/
//#define RT_USING_UART3
//#define RT_USING_UART3_DMA
#endif

#endif
