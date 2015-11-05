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
#include "bsp_spi2.h"
																  
void SPI3_Init(void)
{
	  SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

	  //启动SPI时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    //////下面是SPI相关GPIO初始化//////
    //SPI1模块对应的SCK、MISO、MOSI为AF引脚
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  /* PB12-SPI2-NSS:ENC28J60_CS */ 											 // 片选
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   // 推免输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);						  
	  GPIO_SetBits(GPIOB, GPIO_Pin_12);										 // 先把片选拉高，真正用的时候再拉低
 

		/* SPI2 配置 */ 
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI3, &SPI_InitStructure);
		
		/* Enable SPI2  */
		SPI_Cmd(SPI3, ENABLE); 
}

/*
 * 函数名：SPI2_ReadWrite
 * 描述  ：SPI2读写一字节数据
 * 输入  ： 
 * 输出  ：
 * 返回  ：
 */
unsigned char	SPI2_ReadWrite(unsigned char writedat)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI2, writedat);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}

