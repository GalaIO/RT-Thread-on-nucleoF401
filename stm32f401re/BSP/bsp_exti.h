/*
 *
 *	Author: GalaIO
 *	Date:		2015-7-17 10:53 PM
 *	Description:	
 *		ʹ���ⲿ�жϲ��裺
 *			1.ʹ��IO��ʱ�ӣ���ʼ����IO��ζ����
 *			2.ʹ��SYSCFGʱ�ӣ�����IO�����ж�������ӳ��
 *			3.��ʼ�������жϣ����ô�������
 *			4.�����жϷ��飨NVIC������ʹ���ж�
 *			5.��д�жϺ���
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

//ʹ���ⲿ�ж� ����ʹ��syscfg
#define SYSCFG_ENABLE()	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE)

 //ֻ���������� PINxx���� ����BITxx
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



//�ж��жϷ���
#define isEXTI(BIT)		(EXTI_GetITStatus((BIT))!=RESET)
//����жϱ�־λ
#define clrEXTI(BIT)	EXTI_ClearITPendingBit((BIT))

#endif 
