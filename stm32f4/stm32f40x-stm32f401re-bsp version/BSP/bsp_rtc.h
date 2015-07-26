
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
#define MONDAY		0
#define TUESDAY 	1
#define WEDNESDAY	2
#define THURSDAY	3
#define FRIDAY		4
#define SATURDAY	5
#define SUNDAY		6
extern const uint16_t Date_Benchmark[];

extern const char DATE_WEEK_STR[][10];

typedef struct Data_Time
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
	uint16_t		sub_sec;
}Date_t; 

extern Date_t  rtc_timer;  //定义一个时间结构体变量

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Init_RTC
** 功能描述: RTC初始化
** 参数描述：无
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
u8 Init_RTC(void);

//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
uint8_t Time_Update(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec);

//向RTC更新时间
void Time_Get(void);


//启动RTC
#define RTC_START()	 				(Init_RTC())
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

#endif												   
