#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_	

#include "bsp_user.h"
	
#define ADC_CH5  		5 		 	//ͨ��5	   	    
	   									   
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















