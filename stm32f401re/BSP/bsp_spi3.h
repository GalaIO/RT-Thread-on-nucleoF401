/******************************************************************************
*	Author:		GalaIO
*	Date:			2015-8-22 10:07 PM
*	Description:
*		code the spi3.
*                      Pin assignment:
*             ------------------------------------
*             |       STM32F401x   SPI3        |
*             ------------------------------------
*             | PB10          |   ChipSelect   |
*             | PB5 / MOSI    |   DataIn       |
*             | PB3	/SCK	    |   Clock        |
*             | PB4 / MISO    |   DataOut      |
*             -------------------------------------
*******************************************************************************/
#ifndef _BSP_SPI3_H_
#define _BSP_SPI3_H_

#include "bsp_sys.h"


/*@GalaIO， 定义SPI1的配置 */
#define SET_SPI3_CPOL					SPI_CPOL_Low
#define SET_SPI3_CPHA					SPI_CPHA_1Edge
#define SET_SPI3_BAUDPRE			SPI_BaudRatePrescaler_2
#define SET_SPI3_HIGHSPEED		SPI_BaudRatePrescaler_2
#define SET_SPI3_FIRSTBIT			SPI_FirstBit_MSB

/*
 *初始化SPI3.
**/
void SPI3_Init(void);
/*
 *提高SPI3 读写速度
**/
void SPI3_HightSpeed(void); 
 
/*
 * SPI3读写一个字节（发送完成后返回本次通讯读取的数据）
 *
 * @Param, TxData 待发送的数据
 *
 * @Return, 接收到的数据
 *
**/
u8 SPI3_ReadWrite(u8 TxData);


/*
*a dummy operation for rollup a SPI3 transport.
*/
#define SPI3_DUMMY()		SPI1_ReadWrite(0xFF)

#endif 
