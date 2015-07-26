
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
 *			-handler timer and date tools.
 *				-check date param, correct return 0, erro return 1.
 *					Check_Date(uint16_t year,uint8_t month,uint8_t day)
 *				-check time param, correct return 0, erro return 1.
 *					Check_Time(uint8_t hour,uint8_t min,uint8_t sec)
 *				-get week info.
 *					RTC_Get_Week(uint16_t year,uint16_t month,uint16_t day)
 *				-judge a leap year.
 *					Is_Leap_Year(u16 year)
 *		
**/
#include "bsp_rtc.h"

/*
 *  define and declare.
**/
Date_t  rtc_timer;  //����һ��ʱ��ṹ�����
//�õ�Date_t���������࣬һ����RTC_TimeTypeDef����һ����RTC_DateTypeDef��
#define RTC_GET_DATETYPE()	((RTC_DateTypeDef *)&(rtc_timer.week))
#define RTC_GET_TIMETYPE()	((RTC_TimeTypeDef *)&(rtc_timer.hour))

//�������ʱ������ڵĲ������ֿ���ע������Ĭ����24Сʱ��Ŷ
//���0����ȷ�����1������
uint8_t Check_Date(uint16_t year,uint8_t month,uint8_t day);

uint8_t Check_Time(uint8_t hour,uint8_t min,uint8_t sec);

//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����2000-2099��)
//������������������� 
//����ֵ�����ں�																						 
uint8_t RTC_Get_Week(uint16_t year,uint16_t month,uint16_t day);
//�ж��Ƿ������꺯��
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year);


//�·�   1  2  3  4  5  6  7  8  9  10 11 12  total
//����   31 29 31 30 31 30 31 31 30 31 30 31	 366
//������ 31 28 31 30 31 30 31 31 30 31 30 31   365
//����30�գ���ÿ���µ�ƫ��ʱ���١�
//����   +1 -1 +1  0 +1  0 +1 +1  0 +1  0 +1	 366
//������ +1 -2 +1  0 +1  0 +1 +1  0 +1  0 +1   365
//�·����ݱ�											 
const int8_t	Day_INC_bMon[] = 			{0,1,-1,0,0,1,1,2,3,3,4,4,5};	
const int8_t	Day_INC_bMon_Leap[] = {0,1, 0,1,1,2,2,3,4,4,5,5,6};	  

const uint8_t Month_Table[] = 			{0,31,28,31,30,31,30,31,31,30,31,30,31};	//ƽ����·����ڱ�
const uint8_t Month_Table_Leap[] =  {0,31,29,31,30,31,30,31,31,30,31,30,31};		//������·����ڱ�

//��2000��1��1������6Ϊ��׼��ʶ�����ڡ�ͬʱֻ֧�ִ�2000-2099������ڣ�
//�޸Ļ�׼�����ܸı����ڣ���ֻ����100�꣨RTC���Ƶģ���
const uint16_t Date_Benchmark[] = {2000,1,1,SATURDAY};

//back Time and date.
const uint16_t Bak_Date_Time[] = {2015,7,23,1,55,00};

const char DATE_WEEK_STR[][10] = {"MONDAY","TUESDAY","WEDNESDAY","THURSDAY","FRIDAY","SATURDAY","SUNDAY"};


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Init_RTC
** ��������: RTC��ʼ��
** ������������
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
u8 Init_RTC(void)
{
	RTC_InitTypeDef RTC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //ʹ��PERʱ��
																	//ʹ��PWR��BKP����ʱ��   		
	PWR_BackupAccessCmd(ENABLE);									//ʹ��RTC�ͺ󱸼Ĵ������� 
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5555)						//��ָ���ĺ󱸼Ĵ����ж������ݣ��ж��Ƿ�Ϊ��һ������
	{																
		RCC_LSEConfig(RCC_LSE_ON);									//ʹ���ⲿ����ʱ�� 32.768KHz
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)			//���ָ����RCC��־λ�������,�ȴ����پ������
  		{}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);						//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);										//ʹ��RTCʱ��  
		
				
    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC�첽��Ƶϵ��(1~0X7F) 128-1
    RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTCͬ����Ƶϵ��(0~7FFF) 256-1
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC����Ϊ24Сʱ��
    RTC_Init(&RTC_InitStructure);
		
		//������ԭʼʱ�䣬�����ֻ�û�е�غ�ʱ����ɳ���Ԥ���
		RTC_SET(Bak_Date_Time[0],Bak_Date_Time[1],Bak_Date_Time[2],Bak_Date_Time[3],Bak_Date_Time[4],Bak_Date_Time[5]);			
    RTC_WriteBackupRegister(RTC_BKP_DR0, 0x5555);					//��ָ���ĺ󱸼Ĵ�����д���û���������0X5555���жϱ�־										
	}																 	
	//���ǵ�һ������ ������ʱ
	
	
	return 0; //ok		
}


//�������ʱ������ڵĲ������ֿ���ע������Ĭ����24Сʱ��Ŷ
//���0����ȷ�����1������
uint8_t Check_Date(uint16_t year,uint8_t month,uint8_t day){
	//assert the year .
	if(year>Date_Benchmark[0]+99 || year<Date_Benchmark[0]){
		return 1;
	}
	//assert the month .
	if(month>12 || month<1){
		return 1;
	}
	//assert the day .
	if(Is_Leap_Year(year)){
		if(day<1 || day>Month_Table[month]){
			return 1;
		}
	}else{
		if(day<1 || day>Month_Table_Leap[month]){
			return 1;
		}
	}
	return 0;
}

uint8_t Check_Time(uint8_t hour,uint8_t min,uint8_t sec){
	if(hour>23){
		return 1;
	}
	if(min>59){
		return 1;
	}
	if(sec>59){
		return 1;
	}
	return 0;
}
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
uint8_t Time_Update(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec)
{ 
	uint8_t week;
	if(Check_Date(year,month,day) || Check_Time(hour,min,sec)){
		return 1;
	}
	week = RTC_Get_Week(year,month,day);
	if(week>SUNDAY) return 1;
	rtc_timer.year = year - Date_Benchmark[0];
	rtc_timer.month = month;
	rtc_timer.day = day;
	rtc_timer.week = week;
	rtc_timer.hour = hour;
	rtc_timer.min = min;
	rtc_timer.sec = sec;
	rtc_timer.AMoPM = hour<12?RTC_H12_AM:RTC_H12_PM;
	
	//��RTC����ʱ�������
	RTC_SetTime(RTC_Format_BIN,RTC_GET_TIMETYPE());
	RTC_SetDate(RTC_Format_BIN,RTC_GET_DATETYPE());
	
	return 0;
}

//��RTC����ʱ��
void Time_Get(void){
	//��RTC��ȡʱ�������
	RTC_GetTime(RTC_Format_BIN,RTC_GET_TIMETYPE());
	RTC_GetDate(RTC_Format_BIN,RTC_GET_DATETYPE());

	//��ȡ RTC������
	rtc_timer.sub_sec = RTC_GetSubSecond();
}

//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����2000-2099��)
//������������������� 
//����ֵ�����ں�																						 
uint8_t RTC_Get_Week(uint16_t year,uint16_t month,uint16_t day)
{	
	uint32_t	temp,days;
	//assert the year month day.
	if(Check_Date(year,month,day)){
		return SUNDAY+1;
	}
	//count the number of leap in the range.
	temp=year/4-year/100+year/400-(Date_Benchmark[0]/4-Date_Benchmark[0]/100+Date_Benchmark[0]/400);
	if(Is_Leap_Year(year)){
		temp--;
	}
	days=temp*366+(year-Date_Benchmark[0]-temp-1)*365;		//�õ����߽��� ��ݵ�������
	//����߽� ����.
	//�ұ߽�
	if(Is_Leap_Year(year)){
		days+=(month-1)*30+Day_INC_bMon_Leap[month-1]+day;
	}else{
		days+=(month-1)*30+Day_INC_bMon[month-1]+day;
	}
	//��߽�
	if(Is_Leap_Year(Date_Benchmark[0])){
		days+=(12-Date_Benchmark[1])*30+Day_INC_bMon_Leap[12]-Day_INC_bMon_Leap[Date_Benchmark[1]]+Month_Table_Leap[Date_Benchmark[1]]-Date_Benchmark[2];
	}else{
		days+=(12-Date_Benchmark[1])*30+Day_INC_bMon[12]-Day_INC_bMon[Date_Benchmark[1]]+Month_Table[Date_Benchmark[1]]-Date_Benchmark[2];
	}
	days+=Date_Benchmark[3];
	return days%7;
}
//�ж��Ƿ������꺯��
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}


