
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
	uint16_t		sub_sec;
}RDT_t;

void setDefaultRTC(void);

//sync the date and time to RTC.
//@param pRDT		a point to a RTC struct.
//return 0:success; other:faild.
//notes: the pRDT->year must whtin Date_Benchmark[0] - Date_Benchmark[0]+99, in 100 range.
uint8_t RDT_sync(RDT_t *pRDT);

//gain the newest date and time from RTC.
//@param pRDT	 the RTC struct can contain the whole info.
//return void.
void RDT_gain(RDT_t *pRDT);

//init a RTC.
u8 Init_RTC(void);

//启动RTC quick macro.
#define RTC_START()	 				(Init_RTC())

#endif												   
