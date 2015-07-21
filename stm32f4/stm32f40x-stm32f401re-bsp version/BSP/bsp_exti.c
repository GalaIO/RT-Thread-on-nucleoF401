
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
