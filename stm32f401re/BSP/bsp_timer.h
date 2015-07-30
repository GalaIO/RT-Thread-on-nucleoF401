
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
 * 初始化TIM的PWM通道.
**/
void TIM_PWM_INIT(TIM_TypeDef *TIMx,uint8_t channel,uint32_t duty);

//初始化TIM2定时器，并自动使能定时器，开启溢出中断. 
//@param period  		自动重载计数周期值	0~65535之间
//@param Prescaler	设置预分频器分频系数x，即APB2=y MHz, TIM1_CLK=y/x MHz 
//初始化步骤：
//1.把TIM2外设挂载到APB1总线（低速）上，对于STM32F401是42MHz.
//2.初始化配置TIM2基本参数，设置重载周期值和定时器时钟分频系数.
//3.清除TIM2中断标志。
//4.使能TIM2溢出中断.
//5.使能TIM2.
//6.配置NVIC优先级.
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

//配置定时器 高级宏
//在STM32F401的手册的 时钟树中，写到if(APBx presec = 1) x1 else x2;
//由于APB1是2分频后的低速总线，所以在给timer提供时序时，自动倍频2
#define TIM2_START_US(us)		TIM2_START(us-1,APB1CLK_HZ/1000000*2-1)
#define TIM2_START_MS(ms)		TIM2_START(ms-1,APB1CLK_HZ/1000*2-1)
#define TIM3_START_US(us)		TIM3_START(us-1,APB1CLK_HZ/1000000*2-1)
#define TIM3_START_MS(ms)		TIM3_START(ms-1,APB1CLK_HZ/1000*2-1)

//TIM定时器 溢出判断与清除宏
//判断TIM2中断标志位是否为1；为1则中断响应啦，0则没有
#define isTIM2_IT_UPDATE()	(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//清楚TIM2中断挂起位
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

//TIM2的各PWM通道设置.
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


//TIM3的各PWM通道设置.
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
  不断修正pwm波
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

