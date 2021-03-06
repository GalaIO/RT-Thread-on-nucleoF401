
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
 *					toggle PA1 out status.  
 *						PAOUT_TOG(BIT1)
 *					get PA1 out status.  		
 *						PAOUT_GET(BIT1) 
 *					get PA1 in status. 
 *						PAIN_GET(BIT1)   
 *
**/
#ifndef _HELLO_GPIO_H
#define _HELLO_GPIO_H

#include "bsp_user.h"

//配置GPIO的输出功能。
void POUT_INT(GPIO_TypeDef* GPIOx,u16 BIT);

//配置GPIO的输入功能。
void PIN_INT(GPIO_TypeDef* GPIOx,u16 BIT);

//配置GPIO的复用功能。
void POUT_AF_INT(GPIO_TypeDef* GPIOx,u16 BIT);

//GPIO口的简写
#define BIT0	   GPIO_Pin_0
#define BIT1	   GPIO_Pin_1
#define BIT2	   GPIO_Pin_2
#define BIT3	   GPIO_Pin_3
#define BIT4	   GPIO_Pin_4
#define BIT5	   GPIO_Pin_5
#define BIT6	   GPIO_Pin_6
#define BIT7	   GPIO_Pin_7
#define BIT8	   GPIO_Pin_8
#define BIT9	   GPIO_Pin_9
#define BIT10	   GPIO_Pin_10
#define BIT11	   GPIO_Pin_11
#define BIT12	   GPIO_Pin_12
#define BIT13	   GPIO_Pin_13
#define BIT14	   GPIO_Pin_14
#define BIT15	   GPIO_Pin_15	
#define BITFULL	 GPIO_Pin_All

//GPIO 的PIN管教顺序定义 用于位带操作和设定外部中断
#define PIN0	   GPIO_PinSource0
#define PIN1	   GPIO_PinSource1
#define PIN2	   GPIO_PinSource2
#define PIN3	   GPIO_PinSource3
#define PIN4	   GPIO_PinSource4
#define PIN5	   GPIO_PinSource5
#define PIN6	   GPIO_PinSource6
#define PIN7	   GPIO_PinSource7
#define PIN8	   GPIO_PinSource8
#define PIN9	   GPIO_PinSource9
#define PIN10	   GPIO_PinSource10
#define PIN11	   GPIO_PinSource11
#define PIN12	   GPIO_PinSource12
#define PIN13	   GPIO_PinSource13
#define PIN14	   GPIO_PinSource14
#define PIN15	   GPIO_PinSource15

//定义一个管教位置pin 转 io索引位置的宏
#define _P2B(PIN)  (((u32)0x00001)<<(PIN))

#define PAOUT_INT(BIT)	(POUT_INT(GPIOA,BIT))

#define PBOUT_INT(BIT)	(POUT_INT(GPIOB,BIT))

#define PCOUT_INT(BIT)	(POUT_INT(GPIOC,BIT))

#define PDOUT_INT(BIT)	(POUT_INT(GPIOD,BIT))

#define PEOUT_INT(BIT)	(POUT_INT(GPIOE,BIT))

#define PFOUT_INT(BIT)	(POUT_INT(GPIOF,BIT))

#define PGOUT_INT(BIT)	(POUT_INT(GPIOG,BIT))

//定义io的out的三大函数 设置 清除 翻转
#define PAOUT_SET(BIT)    (GPIO_SetBits(GPIOA,(BIT)))                                  
#define PAOUT_CLR(BIT)    (GPIO_ResetBits(GPIOA,(BIT)))   
#define PAOUT_TOG(BIT)    (GPIO_Write(GPIOA,GPIO_ReadOutputData(GPIOA)^(BIT)))
#define PBOUT_SET(BIT)    (GPIO_SetBits(GPIOB,(BIT)))                                  
#define PBOUT_CLR(BIT)    (GPIO_ResetBits(GPIOB,(BIT)))   
#define PBOUT_TOG(BIT)    (GPIO_Write(GPIOB,GPIO_ReadOutputData(GPIOB)^(BIT)))
#define PCOUT_SET(BIT)    (GPIO_SetBits(GPIOC,(BIT)))                                  
#define PCOUT_CLR(BIT)    (GPIO_ResetBits(GPIOC,(BIT)))   
#define PCOUT_TOG(BIT)    (GPIO_Write(GPIOC,GPIO_ReadOutputData(GPIOC)^(BIT)))
#define PDOUT_SET(BIT)    (GPIO_SetBits(GPIOD,(BIT)))                                  
#define PDOUT_CLR(BIT)    (GPIO_ResetBits(GPIOD,(BIT)))   
#define PDOUT_TOG(BIT)    (GPIO_Write(GPIOD,GPIO_ReadOutputData(GPIOD)^(BIT)))
#define PEOUT_SET(BIT)    (GPIO_SetBits(GPIOE,(BIT)))                                  
#define PEOUT_CLR(BIT)    (GPIO_ResetBits(GPIOE,(BIT)))   
#define PEOUT_TOG(BIT)    (GPIO_Write(GPIOE,GPIO_ReadOutputData(GPIOE)^(BIT)))
#define PFOUT_SET(BIT)    (GPIO_SetBits(GPIOF,(BIT)))                                  
#define PFOUT_CLR(BIT)    (GPIO_ResetBits(GPIOF,(BIT)))   
#define PFOUT_TOG(BIT)    (GPIO_Write(GPIOF,GPIO_ReadOutputData(GPIOF)^(BIT)))
#define PGOUT_SET(BIT)    (GPIO_SetBits(GPIOG,(BIT)))                                  
#define PGOUT_CLR(BIT)    (GPIO_ResetBits(GPIOG,(BIT)))   
#define PGOUT_TOG(BIT)    (GPIO_Write(GPIOG,GPIO_ReadOutputData(GPIOG)^(BIT)))

 //得到相应io口的 以为 或者多位数据 是否有效 返回0 1
#define PAOUT_GET(BIT)  		(GPIO_ReadOutputData(GPIOA)&(BIT)?1:0)
#define PBOUT_GET(BIT)  		(GPIO_ReadOutputData(GPIOB)&(BIT)?1:0)
#define PCOUT_GET(BIT)  		(GPIO_ReadOutputData(GPIOC)&(BIT)?1:0)
#define PDOUT_GET(BIT)  		(GPIO_ReadOutputData(GPIOD)&(BIT)?1:0)
#define PEOUT_GET(BIT)  		(GPIO_ReadOutputData(GPIOE)&(BIT)?1:0)
#define PFOUT_GET(BIT)  		(GPIO_ReadOutputData(GPIOF)&(BIT)?1:0)
#define PGOUT_GET(BIT)  		(GPIO_ReadOutputData(GPIOG)&(BIT)?1:0)

#define PAIN_INT(BIT)	(PIN_INT(GPIOA,BIT))

#define PBIN_INT(BIT)	(PIN_INT(GPIOB,BIT))

#define PCIN_INT(BIT)	(PIN_INT(GPIOC,BIT))

#define PDIN_INT(BIT)	(PIN_INT(GPIOD,BIT))

#define PEIN_INT(BIT)	(PIN_INT(GPIOE,BIT))

#define PFIN_INT(BIT)	(PIN_INT(GPIOF,BIT))

#define PGIN_INT(BIT)	(PIN_INT(GPIOG,BIT))

 //得到相应io口的 以为 或者多位数据 是否有效 返回0 1
#define PAIN_GET(BIT)  		(GPIO_ReadInputData(GPIOA)&(BIT)?1:0)
#define PBIN_GET(BIT)  		(GPIO_ReadInputData(GPIOB)&(BIT)?1:0)
#define PCIN_GET(BIT)  		(GPIO_ReadInputData(GPIOC)&(BIT)?1:0)
#define PDIN_GET(BIT)  		(GPIO_ReadInputData(GPIOD)&(BIT)?1:0)
#define PEIN_GET(BIT)  		(GPIO_ReadInputData(GPIOE)&(BIT)?1:0)
#define PFIN_GET(BIT)  		(GPIO_ReadInputData(GPIOF)&(BIT)?1:0)
#define PGIN_GET(BIT)  		(GPIO_ReadInputData(GPIOG)&(BIT)?1:0)


#endif
