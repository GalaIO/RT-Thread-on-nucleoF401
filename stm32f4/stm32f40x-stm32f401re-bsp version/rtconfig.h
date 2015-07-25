/* RT-Thread config file */
#ifndef __RTTHREAD_CFG_H__
#define __RTTHREAD_CFG_H__

#ifndef RT_NAME_MAX
/* RT_NAME_MAX*/
#define RT_NAME_MAX	8
#endif

#ifndef RT_ALIGN_SIZE
/* RT_ALIGN_SIZE*/
#define RT_ALIGN_SIZE	8
#endif

#ifndef RT_THREAD_PRIORITY_MAX
/* PRIORITY_MAX */
#define RT_THREAD_PRIORITY_MAX	32
#endif

#ifndef RT_TICK_PER_SECOND
/* Tick per Second  100hz 10ms*/
#define RT_TICK_PER_SECOND	100
#endif

#ifndef RT_EVENT_LENGTH
/* event length 			*/
#define RT_EVENT_LENGTH					32
#endif

#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE     1024
#endif

#ifndef RT_USING_TIMER_SOFT
/* Using Software Timer */
/* #define RT_USING_TIMER_SOFT 
#define RT_TIMER_THREAD_PRIO		4
#define RT_TIMER_THREAD_STACK_SIZE	512
#define RT_TIMER_TICK_PER_SECOND	10*/
#endif

/* open RT_DEBUG for RT-Thread*/
#ifndef RT_DEBUG
#define RT_DEBUG
#endif
#endif
