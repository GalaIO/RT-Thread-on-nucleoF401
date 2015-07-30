
/*
 *	Author:		GalaIO
 *	Date:			2015-7-26 10:12 AM
 *	Description:
 *			Re package RTC configuration.
 *			-Define the week description macro.
 *					MONDAY		0
 *						  ....
 *					SUNDAY		6
 *			-init the RTC with Bak_Date_Time[] = {2015,7,23,1,55,00}.
 *				Init_RTC(void)
 *			-update RTC as the specific year month day hour minute second.
 *				Time_Update(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec)
 *			-refresh the time for rtc_timer in sram.
 *				Time_Get(void)
 *			-quick macro for handler RTC and get time info from rtc_timer.
 *				-startup RTC.
 *					RTC_START()
 *				-set y,m,d,h,m,s.
 *					RTC_SET()
 *				-refresh time info from RTC, and wait for query using fllowing macro.
 *					RTC_GET()
 *				-get year,month,day,hour,min,sec,sub_sec.
 *					RTC_YEAR()
 *						....
 *					RTC_SUB_SEC()
 *		
**/
#ifndef _BSP_RTC_H
#define _BSP_RTC_H

#include "bsp_user.h"

//以2000年1月1日星期6为基准来识别星期。同时只支持从2000-2099年的日期，
//修改基准日期能改变日期，但只能数100年（RTC限制的）。
#define MONDAY		1
#define TUESDAY 	2
#define WEDNESDAY	3
#define THURSDAY	4
#define FRIDAY		5
#define SATURDAY	6
#define SUNDAY		7

extern const char DATE_WEEK_STR[][10];

#ifdef RTC_DEVICE_USING_BLOCK

#define RTC_DATE_WEEK_L			0
#define RTC_DATE_MONTH_L		1
#define RTC_DATE_DAY_L			2
#define RTC_DATE_YEAR_L			3
#define RTC_DATE_HOUR_L			4
#define RTC_DATE_MIN_L			5
#define RTC_DATE_SEC_L			6
#define RTC_DATE_APM_L			7
#define RTC_DATE_SUBSEC_L		8

#endif /*RTC_DEVICE_USING_BLOCK*/

#ifdef RTC_DEVICE_USING_SHARED_MEM

extern const uint16_t Date_Benchmark[];
typedef struct Date_Time
{		
	//公历日月年周
	uint8_t 	week;
	uint8_t  	month;
	uint8_t  	day;
	uint8_t  	year;
	//时 分 秒 选择AM/PM
	uint8_t 	hour;
	uint8_t 	min;
	uint8_t 	sec;	
	uint8_t		AMoPM;
	//亚秒(用于精确时间)
	uint8_t		sub_sec;
}Date_t; 

extern Date_t  rtc_timer;  //定义一个时间结构体变量

//update date and time.
uint8_t Time_Update(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec);

//get date and time from RTC.
void Time_Get(void);

//输入年月日 时分秒
#define RTC_SET(sy,sm,sd,h,m,s)		(Time_Update(sy,sm,sd,h,m,s))	

#define RTC_GET()						(Time_Get())

//补偿年份
#define RTC_YEAR()					((uint16_t)rtc_timer.year + Date_Benchmark[0])
#define RTC_WEEK()					(rtc_timer.week)
#define RTC_WEEKSTR()				(DATE_WEEK_STR[rtc_timer.week])
#define RTC_MON()						(rtc_timer.month)
#define RTC_DAY()						(rtc_timer.day)
#define RTC_HOUR()					(rtc_timer.hour)
#define RTC_MIN()						(rtc_timer.min)
#define RTC_SEC()						(rtc_timer.sec)
#define RTC_SUB_SEC()				(rtc_timer.sub_sec)

#endif /*RTC_DEVICE_USING_SHARED_MEM*/


#define RTC_DEVICE_USING_RTC_STRUCT

#ifdef RTC_DEVICE_USING_RTC_STRUCT

typedef struct RTC_DATE_TIME
{		
	//公历日月年周
	uint8_t 	week;
	uint8_t  	month;
	uint8_t  	day;
	uint16_t  year;
	//时 分 秒 选择AM/PM
	uint8_t 	hour;
	uint8_t 	min;
	uint8_t 	sec;	
	uint8_t		AMoPM;
	//亚秒(用于精确时间)
	uint8_t		sub_sec;
}RDT_t;


//sync the date and time to RTC.
//@param pRDT		a point to a RTC struct.
//return 0:success; other:faild.
//notes: the pRDT->year must whtin Date_Benchmark[0] - Date_Benchmark[0]+99, in 100 range.
uint8_t RDT_sync(RDT_t *pRDT);

//gain the newest date and time from RTC.
//@param pRDT	 the RTC struct can contain the whole info.
//return void.
void RDT_gain(RDT_t *pRDT);

#endif /*RTC_DEVICE_USING_RTC_STRUCT*/


//init a RTC.
u8 Init_RTC(void);

//启动RTC quick macro.
#define RTC_START()	 				(Init_RTC())


#endif												   
