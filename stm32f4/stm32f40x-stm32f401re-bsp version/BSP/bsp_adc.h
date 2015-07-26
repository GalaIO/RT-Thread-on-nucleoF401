
/*
 *	Author:		GalaIO
 *	Date:			2015-7-26 10:12 AM
 *	Description:
 *			Re package ADC configuration.
 *			-initiallize the ADC1 ,in STM32F401XE only one ADC,ADC has 17 channel the 16 channel is temp channel.
 *				ADC_INERTEMP_INIT(void)
 *			-get the specific channel value in ADC1.
 *				Get_Adc(uint8_t ch)
 *			-get a filtered data.
 *				Get_Adc_Average(uint8_t ch,uint8_t times)
 *			-get ADC1 iner-temperature.
 *				Get_Temprate(void)
 *		
**/
#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_	

#include "bsp_user.h"
	  	    
	   									   
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//�����¶ȴ�����ͨ��																   
void  ADC_INERTEMP_INIT(void);  
//���ADCֵ
//ch:ͨ��ֵ @ref ADC_channels  0~16��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
uint16_t Get_Adc(uint8_t ch);
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
uint16_t Get_Adc_Average(uint8_t ch,uint8_t times);

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
float Get_Temprate(void);
#endif 















