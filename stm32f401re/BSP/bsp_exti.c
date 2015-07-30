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
#include "bsp_exti.h"

//�ⲿ�жϳ�ʼ��
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
