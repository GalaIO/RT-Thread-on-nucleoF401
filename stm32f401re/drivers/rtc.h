
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

//RTC device name macro.
#define RTC_DEVICE_NAME		"rdt"

//RTC device, control cmd.
#define RTC_CMD_NONE					0x00
#define RTC_CMD_GET_SUBSEC		0x01
#define RTC_CMD_GET_TIME			0x02
#define RTC_CMD_GET_DATE			0x03
#define RTC_CMD_SET_TIME			0x05
#define RTC_CMD_SET_DATE			0x06
#define RTC_CMD_SET_DEFAULT		0x08

//×¢²ártcÉè±¸.
extern rt_err_t rdt_device_register(const char* name);


#endif
