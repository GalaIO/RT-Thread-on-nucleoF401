
/*
 *	Author:		GalaIO
 *	Date:			2015-7-26 10:12 AM
 *	Description:
 *			Re package TIM and TIM_PWM configuration.
 *			-init a TIMx with period and Prescaler param.
 *				TIM_INIT(TIM_TypeDef *TIMx,uint32_t per,uint32_t pre)
 *			-launch a TIMx_PWM_channel whit the duty.
 *			  TIM_PWM_INIT(TIM_TypeDef *TIMx,uint8_t channel,uint32_t duty)
 *
 *
**/
#include "bsp_timer.h"


#define TIM_STRUCT(per,pre){\
TIM_BaseInitStructure.TIM_Period = per;\
TIM_BaseInitStructure.TIM_Prescaler = pre;\
TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;\
TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;\
}

//������װ�Ĵ���ֵ  ��  ����Ƶϵ�� //�Զ���Ԥ��Ƶϵ�����ṩ����ʱ����ʱ��	0~65535֮��
										//����Ԥ��Ƶ����Ƶϵ��71����APB2=72M, TIM1_CLK=72/72=1MHz 
void TIM_INIT(TIM_TypeDef *TIMx,uint32_t per,uint32_t pre){
				
		TIM_TimeBaseInitTypeDef	 TIM_BaseInitStructure;
	
		TIM_STRUCT(per,pre);
		TIM_TimeBaseInit(TIMx, &TIM_BaseInitStructure);
}


/*
 * ��ʼ��TIM��PWMͨ��.
**/
void TIM_PWM_INIT(TIM_TypeDef *TIMx,uint8_t channel,uint32_t duty){
				
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
		TIM_OCStructInit(&TIM_OCInitStructure);
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_Pulse = duty;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		switch(channel){
			case 1:
				TIM_OC1Init(TIMx, &TIM_OCInitStructure);
				break;
			case 2:
				TIM_OC2Init(TIMx, &TIM_OCInitStructure);
				break;
			case 3:
				TIM_OC3Init(TIMx, &TIM_OCInitStructure);
				break;
			default:
				TIM_OC4Init(TIMx, &TIM_OCInitStructure);
		}
		
}
