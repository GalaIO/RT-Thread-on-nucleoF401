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
#include "bsp_exti.h"

//外部中断初始化
#define EXTI_INIT_STRUCTURE(BIT,TYPE){\
EXTI_InitStructure.EXTI_Line = (BIT);\
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;\
EXTI_InitStructure.EXTI_Trigger = (TYPE);\
EXTI_InitStructure.EXTI_LineCmd = ENABLE;\
EXTI_Init(&EXTI_InitStructure);\
} 	

void gpio_exti_init(uint16_t BIT,EXTITrigger_TypeDef TYPE){
		EXTI_InitTypeDef  EXTI_InitStructure;
		EXTI_INIT_STRUCTURE(BIT,TYPE);
}
