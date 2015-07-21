/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
*                      Pin assignment:
*             ----------------------------------------------
*             |  STM32F10x    |     MSD          Pin        |
*             ----------------------------------------------
*             | PA.4          |   ChipSelect      1         |
*             | PA.7 / MOSI   |   DataIn          2         |
*             |               |   GND             3 (0 V)   |
*             |               |   VDD             4 (3.3 V) |
*             | PA.5 / SCLK   |   Clock           5         |
*             |               |   GND             6 (0 V)   |
*             | PA.6 / MISO   |   DataOut         7         |
*             -----------------------------------------------
*******************************************************************************/

#ifndef  _BSP_SPI1_H_
#define  _BSP_SPI1_H_

#include "bsp_gpio.h"
#include "bsp_sys.h"

#define	 SPI1_CS_PORT     GPIOA
#define	 SPI1_CS_PIN	  GPIO_Pin_4
#define	 SPI1_NO		  SPI1
#define	 SPI1_PORT	 	  GPIOA
#define	 SPI1_MISO	 	  GPIO_Pin_6
#define	 SPI1_MOSI	 	  GPIO_Pin_7
#define  SPI1_SCK		  GPIO_Pin_5

//SPI1 进行初始化
void SPI1_Init(void);
 
  
//进行移植时,需要修改
void SPI1_HightSpeed(void);
 
//进行移植时,需要修改
u8 SPI1_ReadWrite(u8 TxData);

#endif
