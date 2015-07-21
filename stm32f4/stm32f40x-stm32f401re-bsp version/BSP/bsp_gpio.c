
#include "bsp_gpio.h"

//����GPIO���������Ϣ
#define GPIO_OUT_MOD_CONF  	      GPIO_Mode_OUT 	 //��ͨ���ģʽ
#define GPIO_OUT_OType_CONF  	  	GPIO_OType_PP  	 //�������
#define GPIO_OUT_SPEED_CONF  	  	GPIO_Speed_50MHz //�ϸ��ٶ�
#define GPIO_OUT_PuPd_CONF  	  	GPIO_PuPd_UP		 //����

#define GPIO_IN_MOD_CONF  	      GPIO_Mode_IN
#define GPIO_IN_OType_CONF  	  	GPIO_OType_PP
#define GPIO_IN_SPEED_CONF  	  	GPIO_Speed_50MHz
#define GPIO_IN_PuPd_CONF  	  		GPIO_PuPd_UP


//��������io�����
#define POUT_TEMP(BIT)	{\
GPIO_InitStructure.GPIO_Pin = (BIT);\
GPIO_InitStructure.GPIO_Mode = GPIO_OUT_MOD_CONF;\
GPIO_InitStructure.GPIO_OType = GPIO_OUT_OType_CONF;\
GPIO_InitStructure.GPIO_Speed = GPIO_OUT_SPEED_CONF;\
GPIO_InitStructure.GPIO_PuPd = GPIO_OUT_PuPd_CONF;\
}

void POUT_INT(GPIO_TypeDef* GPIOx,u16 BIT)	{  
    GPIO_InitTypeDef GPIO_InitStructure;
	POUT_TEMP((BIT));
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

//��������io������	Ĭ�� ��51����
#define PIN_TEMP(BIT)	{\
GPIO_InitStructure.GPIO_Pin = (BIT);\
GPIO_InitStructure.GPIO_Mode = GPIO_IN_MOD_CONF;\
GPIO_InitStructure.GPIO_OType = GPIO_IN_OType_CONF;\
GPIO_InitStructure.GPIO_Speed = GPIO_IN_SPEED_CONF;\
GPIO_InitStructure.GPIO_PuPd = GPIO_IN_PuPd_CONF;\
}

void PIN_INT(GPIO_TypeDef* GPIOx,u16 BIT)	{ 
    GPIO_InitTypeDef GPIO_InitStructure;
	PIN_TEMP((BIT));\
	GPIO_Init(GPIOx, &GPIO_InitStructure);\
}


