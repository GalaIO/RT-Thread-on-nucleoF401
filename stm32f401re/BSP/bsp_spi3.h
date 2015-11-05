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


/*@GalaIO�� ����SPI1������ */
#define SET_SPI3_CPOL					SPI_CPOL_Low
#define SET_SPI3_CPHA					SPI_CPHA_1Edge
#define SET_SPI3_BAUDPRE			SPI_BaudRatePrescaler_2
#define SET_SPI3_HIGHSPEED		SPI_BaudRatePrescaler_2
#define SET_SPI3_FIRSTBIT			SPI_FirstBit_MSB

/*
 *��ʼ��SPI3.
**/
void SPI3_Init(void);
/*
 *���SPI3 ��д�ٶ�
**/
void SPI3_HightSpeed(void); 
 
/*
 * SPI3��дһ���ֽڣ�������ɺ󷵻ر���ͨѶ��ȡ�����ݣ�
 *
 * @Param, TxData �����͵�����
 *
 * @Return, ���յ�������
 *
**/
u8 SPI3_ReadWrite(u8 TxData);


/*
*a dummy operation for rollup a SPI3 transport.
*/
#define SPI3_DUMMY()		SPI1_ReadWrite(0xFF)

#endif 
