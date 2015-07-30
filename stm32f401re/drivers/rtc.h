
/*
 *	Author:		GalaIO
 *	Date:			2015-7-26 10:12 AM
 *	Description:
 *			design a RTC device in RT-Thread.
 *		
**/
#ifndef _RTC_H_
#define _RTC_H_
#include "bsp_user.h"
#include "bsp_rtc.h"

#include "rtthread.h"

#define RTC_DEVICE_USING_RTC_STRUCT

#ifdef RTC_DEVICE_USING_BLOCK
typedef uint16_t rtc_t;
//注册rtc设备.
extern rt_err_t rtc_device_register(const char* name);
#endif

#ifdef RTC_DEVICE_USING_RTC_STRUCT
//注册rtc设备.
extern rt_err_t rdt_device_register(const char* name);

#endif /*RTC_DEVICE_USING_RTC_STRUCT*/

#endif
