/*
 *
 *	Author: GalaIO
 *	Date:		2015-7-17 10:53 PM
 *	Description:	
 *		使用外部中断步骤：
 *			1.使能IO口时钟，初始化该IO口味输入
 *			2.使能SYSCFG时钟，设置IO口与中断西安的映射
 *			3.初始化线上中断，设置触发条件
 *			4.配置中断分组（NVIC），并使能中断
 *			5.编写中断函数
 *
*/
#ifndef _HELLO_EXTI_H
#define _HELLO_EXTI_H

#include "bsp_sys.h"
#include "bsp_gpio.h"

#define EXTI_R 	 			EXTI_Trigger_Rising
#define EXTI_F 	 			EXTI_Trigger_Falling
#define EXTI_FR 	 		EXTI_Trigger_Rising_Falling

extern void gpio_exti_init(uint16_t BIT,EXTITrigger_TypeDef TYPE);

//使用外部中断 必须使能syscfg
#define SYSCFG_ENABLE()	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE)

 //只有在这输入 PINxx其他 都是BITxx
#define  PAEXTI_INT(PIN,TYPE){\
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,(PIN));\
gpio_exti_init(_P2B(PIN),TYPE);\
}

#define  PBEXTI_INT(PIN,TYPE){\
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,(PIN));\
gpio_exti_init(_P2B(PIN),TYPE);\
}

#define  PCEXTI_INT(PIN,TYPE){\
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,(PIN));\
gpio_exti_init(_P2B(PIN),TYPE);\
}

#define  PDEXTI_INT(PIN,TYPE){\
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,(PIN));\
gpio_exti_init(_P2B(PIN),TYPE);\
}

#define  PEEXTI_INT(PIN,TYPE){\
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,(PIN));\
gpio_exti_init(_P2B(PIN),TYPE);\
}

#define  PFEXTI_INT(PIN,TYPE){\
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,(PIN));\
gpio_exti_init(_P2B(PIN),TYPE);\
}

#define  PGEXTI_INT(PIN,TYPE){\
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG,(PIN));\
gpio_exti_init(_P2B(PIN),TYPE);\
}



//判断中断发生
#define isEXTI(BIT)		(EXTI_GetITStatus((BIT))!=RESET)
//清除中断标志位
#define clrEXTI(BIT)	EXTI_ClearITPendingBit((BIT))

#endif 
