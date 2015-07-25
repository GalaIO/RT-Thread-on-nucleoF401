#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_	

#include "bsp_user.h"
	
#define ADC_CH5  		5 		 	//通道5	   	    
	   									   
//初始化ADC
//这里我们仅以规则通道为例
//开启温度传感器通道																   
void  ADC_INERTEMP_INIT(void);  
//获得ADC值
//ch:通道值 @ref ADC_channels  0~16：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
uint16_t Get_Adc(uint8_t ch);
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
uint16_t Get_Adc_Average(uint8_t ch,uint8_t times);

//得到温度值
//返回值:温度值(扩大了100倍,单位:℃.)
float Get_Temprate(void);
#endif 















