/******************************************************************************
*	Author:		GalaIO
*	Date:			2015-8-22 10:07 PM
*	Description:
*		code the spi1.
*                      Pin assignment:
*             ------------------------------------
*             |       STM32F401x   SPI1        |
*             ------------------------------------
*             | PB6           |   ChipSelect   |
*             | PA7 / MOSI    |   DataIn       |
*             | PA5	/SCK	    |   Clock        |
*             | PA6 / MISO    |   DataOut      |
*             -------------------------------------
*******************************************************************************/

#ifndef  _BSP_SPI1_H_
#define  _BSP_SPI1_H_

#include "bsp_gpio.h"
#include "bsp_sys.h"

/*@GalaIO， 定义SPI1的配置 */
#define SET_SPI1_CPOL					SPI_CPOL_Low
#define SET_SPI1_CPHA					SPI_CPHA_1Edge
#define SET_SPI1_BAUDPRE			SPI_BaudRatePrescaler_8
#define SET_SPI1_HIGHSPEED		SPI_BaudRatePrescaler_8
#define SET_SPI1_FIRSTBIT			SPI_FirstBit_MSB

/*
 *初始化SPI1.
**/
void SPI1_Init(void);
/*
 *提高SPI1 读写速度
**/
void SPI1_HightSpeed(void); 
 
/*
 * SPI读写一个字节（发送完成后返回本次通讯读取的数据）
 *
 * @Param, TxData 待发送的数据
 *
 * @Return, 接收到的数据
 *
**/
u8 SPI1_ReadWrite(u8 TxData);


/*
*a dummy operation for rollup a SPI transport.
*/
#define SPI1_DUMMY()		SPI1_ReadWrite(0xFF)

#endif
