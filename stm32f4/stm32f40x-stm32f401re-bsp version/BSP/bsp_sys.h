
/*
 *	Author:		GalaIO
 *	Date:			2015-7-26 10:12 AM
 *	Description:
 *			define the delay call.
 *			-delay count ms , and do nothing in the meanwhile.
 *				delayMS(int count)
 *			-delay count us , and do nothing in the meanwhile.
 *				void delayUS(int count)
**/
#ifndef _BSP_SYS_H
#define _BSP_SYS_H

#include "bsp_user.h"

//延迟ms任务（损害cpu资源）
extern void delayMS(int count);
//延迟us任务（损害cpu资源）
extern void delayUS(int count);


#endif
