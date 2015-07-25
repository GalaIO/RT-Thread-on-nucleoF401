
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

//back Time and date.
extern const uint16_t Bak_Date_Time[];
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

//得到Date_t的两个父类，一个叫RTC_TimeTypeDef；另一个是RTC_DateTypeDef。
#define RTC_GET_DATETYPE()	((RTC_DateTypeDef *)&(rtc_timer.week))
#define RTC_GET_TIMETYPE()	((RTC_TimeTypeDef *)&(rtc_timer.hour))

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Init_RTC
** 功能描述: RTC初始化
** 参数描述：无
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
u8 Init_RTC(void);

//检查输入时间和日期的参数，分开，注意我们默认是24小时制哦
//输出0，正确；输出1，错误。
uint8_t Check_Date(uint16_t year,uint8_t month,uint8_t day);

uint8_t Check_Time(uint8_t hour,uint8_t min,uint8_t sec);
//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
uint8_t Time_Update(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec);

//向RTC更新时间
void Time_Get(void);

//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许2000-2099年)
//输入参数：公历年月日 
//返回值：星期号																						 
uint8_t RTC_Get_Week(uint16_t year,uint16_t month,uint16_t day);
//判断是否是闰年函数
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year);


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
