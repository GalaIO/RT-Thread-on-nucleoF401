
/*
 *	Author:		GalaIO
 *	Date:			2015-7-26 12:21 PM
 *	Description:
 *			Re package STD-gpio lib.
 *			-init a gpio funtion.
 *				eg. init a io for output.
 *								POUT_INT(GPIOA,BIT1);
 *						init a io for input.
 *								PIN_INT(GPIOA,BIT1);
 *						init a io for AF-alertion function.
 *								POUT_AF_INT(GPIOA,BIT2);
 *						init a io for Analog funtion.
 *								POUT_ADCIN_INT(GPIOA,BIT1);
 *				-handler a gpio.
 *					set PA1 out 1.
 *						PAOUT_SET(BIT1) 
 *					set PA1 out 0.                              
 *						PAOUT_CLR(BIT1) 
 *					toggle PA1 out 0.  
 *						PAOUT_TOG(BIT1)
 *					get PA1 out status.  		
 *						PAOUT_GET(BIT1) 
 *					get PA1 in status. 
 *						PAIN_GET(BIT1)   
 *
**/
#include "bsp_gpio.h"

//配置GPIO输入输出信息
#define GPIO_OUT_MOD_CONF  	      GPIO_Mode_OUT 	 //普通输出模式
#define GPIO_OUT_OType_CONF  	  	GPIO_OType_PP  	 //推挽输出
#define GPIO_OUT_SPEED_CONF  	  	GPIO_Speed_50MHz //较高速度
#define GPIO_OUT_PuPd_CONF  	  	GPIO_PuPd_UP		 //上拉

#define GPIO_IN_MOD_CONF  	      GPIO_Mode_IN
#define GPIO_IN_OType_CONF  	  	GPIO_OType_PP
#define GPIO_IN_SPEED_CONF  	  	GPIO_Speed_50MHz
#define GPIO_IN_PuPd_CONF  	  		GPIO_PuPd_UP


//快速配置io的输出
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

//配置GPIO的复用功能。
void POUT_AF_INT(GPIO_TypeDef* GPIOx,u16 BIT)	{  
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = (BIT);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOx, &GPIO_InitStructure);
}


//配置GPIO的ADC输入功能。
void POUT_ADCIN_INT(GPIO_TypeDef* GPIOx,u16 BIT)	{  
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = (BIT);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOx, &GPIO_InitStructure);
}


//快速配置io的输入	默认 如51上拉
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


