/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                           (c) Copyright 1992-2007, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                  uC/OS-II Configuration File for V2.8x
*
* File    : OS_CFG.H
* By      : Jean J. Labrosse
* Version : V2.85
*
* LICENSING TERMS:
* ---------------
*   uC/OS-II is provided in source form for FREE evaluation, for educational use or for peaceful research.
* If you plan on using  uC/OS-II  in a commercial product you need to contact Micri祄 to properly license
* its use in your product. We provide ALL the source code for your convenience and to help you experience
* uC/OS-II.   The fact that the  source is provided does  NOT  mean that you can use it without  paying a
* licensing fee.
*********************************************************************************************************
*/

#ifndef OS_CFG_H
#define OS_CFG_H

                                       /* ---------------------- MISCELLANEOUS ----------------------- */
//应用函数钩子函数
//#define OS_APP_HOOKS_EN           1    /* Application-defined hooks are called from the uC/OS-II hooks */  
#define OS_APP_HOOKS_EN           0    /* ---------------------禁用应用软件钩子函数----------------- */
#define OS_ARG_CHK_EN             1    /* Enable (1) or Disable (0) argument checking                  */
//cpu钩子函数
#define OS_CPU_HOOKS_EN           1    /* uC/OS-II hooks are found in the processor port files         */

//调式
//#define OS_DEBUG_EN               1    /* Enable(1) debug variables     							   */
#define OS_DEBUG_EN               0    /* ---------------禁用调试-----     							   */
//使能多重事件控制                               
//#define OS_EVENT_MULTI_EN         1    /* Include code for OSEventPendMulti()                          */
#define OS_EVENT_MULTI_EN         0    /* ------------------禁用多重事件控制----------------         */

//名字 统一 使用4字节
#define OS_EVENT_NAME_SIZE       4    /* Determine the size of the name of a Sem, Mutex, Mbox or Q    */

/*
在UCOSII中，原则上不同人物的优先级是不同的，而且在UCOSII2.8中，
增加了任务数，不超过254，同时限制了任务数和最低任务优先级的关系
OS_MAX_TASKS must be <= OS_LOWEST_PRIO - OS_N_SYS_TASKS + 1

在uCOSII初始化的时候最低优先级 OS_LOWEST_PRIO总是被赋给空闲任务idle task
*/

#define OS_LOWEST_PRIO           14    /* Defines the lowest priority that can be assigned ...         */
                                       /* ... MUST NEVER be higher than 254!                           */
#if     OS_LOWEST_PRIO < 4
    #error  "OS_CFG.H,         OS_LOWEST_PRIO must be >= 4"
#endif


#define OS_MAX_EVENTS            (OS_LOWEST_PRIO-2+1)    /* Max. number of event control blocks in your application      */
#define OS_MAX_FLAGS              5    /* Max. number of Event Flag Groups    in your application      */
#define OS_MAX_MEM_PART           5    /* Max. number of memory partitions                             */
#define OS_MAX_QS                 4    /* Max. number of queue control blocks in your application      */
/*
默认在uCOSII中 有两个特殊任务，
一个是空闲任务idle task 
一个是统计任务 statistics task 
一般都开启，这两个任务也是要占据优先级的 
所以必须减去2，而且OS_MAX_TASKS与OS_LOWEST_PRIO有关
*/
#define OS_MAX_TASKS      (OS_LOWEST_PRIO - 2)    /* Max. number of tasks in your application, MUST be >= 2       */

//使能调度锁
#define OS_SCHED_LOCK_EN          1    /*     Include code for OSSchedLock() and OSSchedUnlock()       */

//设置系统滴答时钟 使能OS_TICK_STEP_EN    滴答时钟频率	OS_TICKS_PER_SEC 单位hz
#define OS_TICK_STEP_EN           1    /* Enable tick stepping feature for uC/OS-View                  */
#define OS_TICKS_PER_SEC       1000    /* Set the number of ticks in one second                        */

// 设置栈的大小
                                       /* --------------------- TASK STACK SIZE ---------------------- */
#define OS_TASK_TMR_STK_SIZE    128    /* Timer      task stack size (# of OS_STK wide entries)        */
#define OS_TASK_STAT_STK_SIZE   128    /* Statistics task stack size (# of OS_STK wide entries)        */
#define OS_TASK_IDLE_STK_SIZE   128    /* Idle       task stack size (# of OS_STK wide entries)        */

/*
  以上是ucos系统的基本配置信息 	 
  以下是ucos系统内核各部分功能配置 任务。。信号量，，	邮箱，，内存管理，，队列，，等等
*/
                                       /* ------------任务管理-- TASK MANAGEMENT ---------------------- */
#define OS_TASK_CHANGE_PRIO_EN    1    /*     Include code for OSTaskChangePrio()                      */
#define OS_TASK_CREATE_EN         1    /*     Include code for OSTaskCreate()                          */
#define OS_TASK_CREATE_EXT_EN     1    /*     Include code for OSTaskCreateExt()                       */
#define OS_TASK_DEL_EN            1    /*     Include code for OSTaskDel()                             */
#define OS_TASK_NAME_SIZE         4    /*     Determine the size of a task name                        */
#define OS_TASK_PROFILE_EN        1    /*     Include variables in OS_TCB for profiling                */
#define OS_TASK_QUERY_EN          1    /*     Include code for OSTaskQuery()                           */
#define OS_TASK_STAT_EN           1    /*     Enable (1) or Disable(0) the statistics task             */
#define OS_TASK_STAT_STK_CHK_EN   1    /*     Check task stacks from statistic task                    */
#define OS_TASK_SUSPEND_EN        1    /*     Include code for OSTaskSuspend() and OSTaskResume()      */
#define OS_TASK_SW_HOOK_EN        1    /*     Include code for OSTaskSwHook()                          */


                                       /* -----------信号量集----- EVENT FLAGS ------------------------ */
//#define OS_FLAG_EN                1    /* Enable (1) or Disable (0) code generation for EVENT FLAGS    */
#define OS_FLAG_EN                0    /*-------------------禁用信号量集------------------------------*/
#define OS_FLAG_ACCEPT_EN         1    /*     Include code for OSFlagAccept()                          */
#define OS_FLAG_DEL_EN            1    /*     Include code for OSFlagDel()                             */
#define OS_FLAG_NAME_SIZE         4    /*     Determine the size of the name of an event flag group    */
#define OS_FLAGS_NBITS           16    /*     Size in #bits of OS_FLAGS data type (8, 16 or 32)        */
#define OS_FLAG_QUERY_EN          1    /*     Include code for OSFlagQuery()                           */
#define OS_FLAG_WAIT_CLR_EN       1    /*     Include code for Wait on Clear EVENT FLAGS               */


                                       /* ------消息邮箱------- MESSAGE MAILBOXES --------------------- */
//#define OS_MBOX_EN                1    /* Enable (1) or Disable (0) code generation for MAILBOXES      */  
#define OS_MBOX_EN                0    /* -------------------禁用邮箱-----------------------------     */
#define OS_MBOX_ACCEPT_EN         1    /*     Include code for OSMboxAccept()                          */
#define OS_MBOX_DEL_EN            1    /*     Include code for OSMboxDel()                             */
#define OS_MBOX_PEND_ABORT_EN     1    /*     Include code for OSMboxPendAbort()                       */
#define OS_MBOX_POST_EN           1    /*     Include code for OSMboxPost()                            */
#define OS_MBOX_POST_OPT_EN       1    /*     Include code for OSMboxPostOpt()                         */
#define OS_MBOX_QUERY_EN          1    /*     Include code for OSMboxQuery()                           */


                                       /* -------内存管理----- MEMORY MANAGEMENT -------------------- */
//#define OS_MEM_EN                 1    /* Enable (1) or Disable (0) code generation for MEMORY MANAGER */ 
#define OS_MEM_EN                 0    /* -------------------禁用内存管理------------------------------ */
#define OS_MEM_NAME_SIZE          4    /*     Determine the size of a memory partition name            */
#define OS_MEM_QUERY_EN           1    /*     Include code for OSMemQuery()                            */


                                       /* -----互斥信号量--- MUTUAL EXCLUSION SEMAPHORES --------------- */
//#define OS_MUTEX_EN               1    /* Enable (1) or Disable (0) code generation for MUTEX          */
#define OS_MUTEX_EN               0    /* ---------------------禁用互斥信号量---------------          */
#define OS_MUTEX_ACCEPT_EN        1    /*     Include code for OSMutexAccept()                         */
#define OS_MUTEX_DEL_EN           1    /*     Include code for OSMutexDel()                            */
#define OS_MUTEX_QUERY_EN         1    /*     Include code for OSMutexQuery()                          */


                                       /* -------队列--------- MESSAGE QUEUES ---------------------- */
//#define OS_Q_EN                   1    /* Enable (1) or Disable (0) code generation for QUEUES         */
#define OS_Q_EN                   0    /* ---------------------禁用队列----------------------------   */
#define OS_Q_ACCEPT_EN            1    /*     Include code for OSQAccept()                             */
#define OS_Q_DEL_EN               1    /*     Include code for OSQDel()                                */
#define OS_Q_FLUSH_EN             1    /*     Include code for OSQFlush()                              */
#define OS_Q_PEND_ABORT_EN        1    /*     Include code for OSQPendAbort()                          */
#define OS_Q_POST_EN              1    /*     Include code for OSQPost()                               */
#define OS_Q_POST_FRONT_EN        1    /*     Include code for OSQPostFront()                          */
#define OS_Q_POST_OPT_EN          1    /*     Include code for OSQPostOpt()                            */
#define OS_Q_QUERY_EN             1    /*     Include code for OSQQuery()                              */


                                       /* -------信号量------- SEMAPHORES ------------------------ */
#define OS_SEM_EN                 1    /* Enable (1) or Disable (0) code generation for SEMAPHORES     */ 
//#define OS_SEM_EN                 0    /* --------------------禁用信号量--------------------------   */
#define OS_SEM_ACCEPT_EN          1    /*    Include code for OSSemAccept()                            */
#define OS_SEM_DEL_EN             1    /*    Include code for OSSemDel()                               */
#define OS_SEM_PEND_ABORT_EN      1    /*    Include code for OSSemPendAbort()                         */
#define OS_SEM_QUERY_EN           1    /*    Include code for OSSemQuery()                             */
#define OS_SEM_SET_EN             1    /*    Include code for OSSemSet()                               */


                                       /* ----时间管理--------- TIME MANAGEMENT ---------------------- */
#define OS_TIME_DLY_HMSM_EN       1    /*     Include code for OSTimeDlyHMSM()                         */
#define OS_TIME_DLY_RESUME_EN     1    /*     Include code for OSTimeDlyResume()                       */
#define OS_TIME_GET_SET_EN        1    /*     Include code for OSTimeGet() and OSTimeSet()             */
#define OS_TIME_TICK_HOOK_EN      1    /*     Include code for OSTimeTickHook()                        */


                                       /* -----定时器管理--------- TIMER MANAGEMENT --------------------- */
//#define OS_TMR_EN                 1    /* Enable (1) or Disable (0) code generation for TIMERS         */ 
#define OS_TMR_EN                 0    /* -------------------禁用定时器 -----------------------       */
#define OS_TMR_CFG_MAX           16    /*     Maximum number of timers                                 */
#define OS_TMR_CFG_NAME_SIZE      4    /*     Determine the size of a timer name                       */
#define OS_TMR_CFG_WHEEL_SIZE     8    /*     Size of timer wheel (#Spokes)                            */
#define OS_TMR_CFG_TICKS_PER_SEC 10    /*     Rate at which timer management task runs (Hz)            */


#endif
