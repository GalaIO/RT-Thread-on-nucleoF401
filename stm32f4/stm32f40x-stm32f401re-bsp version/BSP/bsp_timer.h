
/*
 *	Author:		GalaIO
 *	Date:			2015-7-26 10:12 AM
 *	Description:
 *			Re package TIM and TIM_PWM configuration.
 *			-init a TIMx with period and Prescaler param.
 *				TIM_INIT(TIM_TypeDef *TIMx,uint32_t per,uint32_t pre)
 *			-launch a TIMx_PWM_channel whit the duty.
 *			  TIM_PWM_INIT(TIM_TypeDef *TIMx,uint8_t channel,uint32_t duty)
 *			-quick macro for init a TIMx with period and Prescaler param, enable IT_update , cmd right now.
 *				TIM2_START(per,pre)
 *				TIM3_START
 *			-quick macro for launch the specific PWM channel.
 *				TIM2_PWM_PA0(duty)
 *							.....
 *			-note: quick macro only support TIM2 and TIM3.
 *
**/
#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

#include "bsp_sys.h"
#include "bsp_gpio.h"

void TIM_INIT(TIM_TypeDef *TIMx,uint32_t per,uint32_t pre);

/*
 * ��ʼ��TIM��PWMͨ��.
**/
void TIM_PWM_INIT(TIM_TypeDef *TIMx,uint8_t channel,uint32_t duty);

//��ʼ��TIM2��ʱ�������Զ�ʹ�ܶ�ʱ������������ж�. 
//@param period  		�Զ����ؼ�������ֵ	0~65535֮��
//@param Prescaler	����Ԥ��Ƶ����Ƶϵ��x����APB2=y MHz, TIM1_CLK=y/x MHz 
//��ʼ�����裺
//1.��TIM2������ص�APB1���ߣ����٣��ϣ�����STM32F401��42MHz.
//2.��ʼ������TIM2����������������������ֵ�Ͷ�ʱ��ʱ�ӷ�Ƶϵ��.
//3.���TIM2�жϱ�־��
//4.ʹ��TIM2����ж�.
//5.ʹ��TIM2.
//6.����NVIC���ȼ�.
#define TIM2_START(per,pre)	{\
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);\
TIM_INIT(TIM2,per,pre);\
TIM_ClearFlag(TIM2, TIM_FLAG_Update);\
TIM_ARRPreloadConfig(TIM2, DISABLE);\
TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);\
TIM_Cmd(TIM2, ENABLE);\
GENERAL_NVIC_CMD(TIM2_IRQn);\
}

#define TIM3_START(per,pre)	{\
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);\
TIM_INIT(TIM3,per,pre);\
TIM_ClearFlag(TIM3, TIM_FLAG_Update);\
TIM_ARRPreloadConfig(TIM3, DISABLE);\
TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);\
TIM_Cmd(TIM3, ENABLE);\
GENERAL_NVIC_CMD(TIM3_IRQn);\
}

//���ö�ʱ�� �߼���
//��STM32F401���ֲ�� ʱ�����У�д��if(APBx presec = 1) x1 else x2;
//����APB1��2��Ƶ��ĵ������ߣ������ڸ�timer�ṩʱ��ʱ���Զ���Ƶ2
#define TIM2_START_US(us)		TIM2_START(us-1,APB1CLK_HZ/1000000*2-1)
#define TIM2_START_MS(ms)		TIM2_START(ms-1,APB1CLK_HZ/1000*2-1)
#define TIM3_START_US(us)		TIM3_START(us-1,APB1CLK_HZ/1000000*2-1)
#define TIM3_START_MS(ms)		TIM3_START(ms-1,APB1CLK_HZ/1000*2-1)

//TIM��ʱ�� ����ж��������
//�ж�TIM2�жϱ�־λ�Ƿ�Ϊ1��Ϊ1���ж���Ӧ����0��û��
#define isTIM2_IT_UPDATE()	(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//���TIM2�жϹ���λ
#define clrTIM2_IT_UPDATE()	TIM_ClearITPendingBit(TIM2, TIM_IT_Update)

#define isTIM3_IT_UPDATE()	(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
#define clrTIM3_IT_UPDATE()	TIM_ClearITPendingBit(TIM3, TIM_IT_Update)

/*
  TIM2_CH1--------PA0
  TIM2_CH2--------PA1 
  TIM2_CH3--------PB10

  TIM3_CH1--------PB4 
  TIM3_CH2--------PB5  
  TIM3_CH3--------PB0	

*/

//TIM2�ĸ�PWMͨ������.
#define TIM2_PWM_PA0(duty){\
GPIO_PinAFConfig(GPIOA,PIN0,GPIO_AF_TIM2);\
POUT_AF_INT(GPIOA,BIT0);\
TIM_PWM_INIT(TIM2,(uint8_t)1,duty);\
TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);\
}
#define TIM2_PWM_PA1(duty){\
GPIO_PinAFConfig(GPIOA,PIN1,GPIO_AF_TIM2);\
POUT_AF_INT(GPIOA,BIT1);\
TIM_PWM_INIT(TIM2,(uint8_t)2,duty);\
TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);\
}
#define TIM2_PWM_PB10(duty){\
GPIO_PinAFConfig(GPIOB,PIN10,GPIO_AF_TIM2);\
POUT_AF_INT(GPIOB,BIT10);\
TIM_PWM_INIT(TIM2,(uint8_t)3,duty);\
TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);\
}


//TIM3�ĸ�PWMͨ������.
#define TIM3_PWM_PB4(duty){\
GPIO_PinAFConfig(GPIOB,PIN4,GPIO_AF_TIM3);\
POUT_AF_INT(GPIOB,BIT4);\
TIM_PWM_INIT(TIM3,(uint8_t)1,duty);\
TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);\
}
#define TIM3_PWM_PB5(duty){\
GPIO_PinAFConfig(GPIOB,PIN5,GPIO_AF_TIM3);\
POUT_AF_INT(GPIOB,BIT5);\
TIM_PWM_INIT(TIM3,(uint8_t)2,duty);\
TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);\
}
#define TIM3_PWM_PB0(duty){\
GPIO_PinAFConfig(GPIOB,PIN0,GPIO_AF_TIM3);\
POUT_AF_INT(GPIOB,BIT0);\
TIM_PWM_INIT(TIM3,(uint8_t)3,duty);\
TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);\
}

/*
  ��������pwm��
*/

/*
  TIM2_CH1--------PA0
  TIM2_CH2--------PA1 
  TIM2_CH3--------PB10

  TIM3_CH1--------PB4 
  TIM3_CH2--------PB5  
  TIM3_CH3--------PB0	

*/
#define TIM2_PWM_PA0_SET(duty)	(TIM_SetCompare1(TIM2,duty)) 
#define TIM2_PWM_PA1_SET(duty)	(TIM_SetCompare2(TIM2,duty))
#define TIM2_PWM_PB10_SET(duty)	(TIM_SetCompare3(TIM2,duty))

														 
#define TIM2_PWM_PB4_SET(duty)	(TIM_SetCompare1(TIM3,duty)) 
#define TIM2_PWM_PB5_SET(duty)	(TIM_SetCompare2(TIM3,duty))  
#define TIM2_PWM_PB0_SET(duty)	(TIM_SetCompare3(TIM3,duty)) 


#endif

