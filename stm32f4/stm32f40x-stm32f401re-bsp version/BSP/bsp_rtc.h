
#ifndef _HELLO_RTC_H
#define _HELLO_RTC_H

#include "bsp_user.h"

typedef struct Data_Time
{
	u8 hour;
	u8 min;
	u8 sec;			
	//公历日月年周
	u16 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;
}Date_; 

extern Date_  rtc_timer;  //定义一个时间结构体变量

extern u8 Init_RTC(void);
extern u8 Is_Leap_Year(u16 year);
extern u8 Time_Update(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
extern u8 Time_Get(void);
extern u8 RTC_Get_Week(u16 year,u8 month,u8 day);
extern void RTC_NVIC_Config(void);
//启动RTC
#define RTC_START()	 				(Init_RTC())
//输入年月日 时分秒
#define RTC_SET(sy,sm,sd,h,m,s)		(Time_Update(sy,sm,sd,h,m,s))	

#define RTC_GET()					(Time_Get())

#define RTC_YEAR()					(rtc_timer.w_year)
#define RTC_WEEK()					(rtc_timer.week)
#define RTC_MON()					(rtc_timer.w_month)
#define RTC_DAY()					(rtc_timer.w_date)
#define RTC_HOUR()					(rtc_timer.hour)
#define RTC_MIN()					(rtc_timer.min)
#define RTC_SEC()					(rtc_timer.sec)

#endif												   
