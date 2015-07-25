
#include "bsp_adc.h"

//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//�����¶ȴ�����ͨ��																   
void  ADC_INERTEMP_INIT(void)
{    
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//ʹ��ADC1ʱ�� 
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
  ADC_TempSensorVrefintCmd(ENABLE);//ʹ���ڲ��¶ȴ�����
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; //ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC1, &ADC_InitStructure);
 
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_480Cycles );	//ADC5,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��		
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles );	//ADC16,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��		
	ADC_Cmd(ADC1, ENABLE);//����ADת����	 			
 
}				  
//���ADCֵ
//ch:ͨ��ֵ @ref ADC_channels  0~16��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
uint16_t Get_Adc(uint8_t ch)   
{
	 //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
uint16_t Get_Adc_Average(uint8_t ch,uint8_t times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++){
		temp_val+=Get_Adc(ch);//��ȡͨ��ת��ֵ
	}
	return temp_val/times;
} 

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
float Get_Temprate(void)
{
	u32 adcx;
 	double temperate;
	adcx=Get_Adc_Average(ADC_Channel_16,10);	//��ȡͨ��16�ڲ��¶ȴ�����ͨ��,10��ȡƽ��
	temperate=(float)adcx*(3.3/4096);		//��ѹֵ
	temperate=(temperate-0.76)/0.0025 + 25; //ת��Ϊ�¶�ֵ 
	return temperate;
}
	 










