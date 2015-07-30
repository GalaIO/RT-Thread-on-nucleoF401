
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

//�ӳ�ms������cpu��Դ��
extern void delayMS(int count);
//�ӳ�us������cpu��Դ��
extern void delayUS(int count);


#endif
