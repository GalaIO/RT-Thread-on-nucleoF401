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
#include "bsp_spi3.h"
#include "bsp_user.h"
#include "bsp_gpio.h"
																  
void SPI3_Init(void)
{
	  SPI_InitTypeDef  SPI_InitStructure;

	  //启动SPI时钟
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3, ENABLE);
    //////下面是SPI相关GPIO初始化//////
		RCC_CMD(PB_PER);
    //SPI1模块对应的SCK、MISO、MOSI为AF引脚
		POUT_AF_INT(GPIOB,BIT3|BIT4|BIT5);
    GPIO_PinAFConfig(GPIOB,PIN3,GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOB,PIN4,GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOB,PIN5,GPIO_AF_SPI3);
	
	  /* PB10-SPI3-NSS:ENC28J60_CS */ 											 // 片选
    PBOUT_INT(BIT10);
		//pull up
		PBOUT_SET(BIT10);
	
	  //////SPI模块配置//////
		//SPI_Direction 是用来设置 SPI 的通信方式，可以选择为半双工，全双工，以及串行发和串行收方式，
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		// SPI_Mode 用来设置 SPI 的主从模式，
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		//SPI_DataSiz 为 8 位还是 16 位帧格式选择项
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		//SPI_CPOL 用来设置时钟极性，设置串行同步时钟的空闲状态为高或低电平。
  	SPI_InitStructure.SPI_CPOL = SET_SPI3_CPOL;
		// SPI_CPHA 用来设置时钟相位，也就是选择在串行同步时钟的第几个跳变沿（上升或下降）数据被采样，可以为第一个或者第二个条边沿采集。
  	SPI_InitStructure.SPI_CPHA = SET_SPI3_CPHA;
		//设置 NSS 信号由硬件（ NSS 管脚）还是软件控制。
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		//设置 SPI 波特率预分频值也就是决定 SPI 的时钟的参数，从 2 分频到 256 分频 8 个可选值，决定了SPI传输速度和时钟间隔。
  	SPI_InitStructure.SPI_BaudRatePrescaler = SET_SPI3_BAUDPRE;
		//设置数据传输顺序是 MSB 位在前还是 LSB 位在前，不同SPI子设备要求不同。
  	SPI_InitStructure.SPI_FirstBit = SET_SPI3_FIRSTBIT;
		//用来设置 CRC 校验多项式，提高通信可靠性，大于 1 即可。
  	SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI3,&SPI_InitStructure);
		//启动 SPI1。
    SPI_Cmd(SPI3,ENABLE);
		SPI3_ReadWrite(0xff);//启动传输	
}

  
/*
 *提高SPI3 读写速度
**/
void SPI3_HightSpeed(void) 
{
	  SPI_InitTypeDef SPI_InitStructure ;

    SPI_Cmd(SPI3,DISABLE);

    //////SPI模块配置//////
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  	SPI_InitStructure.SPI_CPOL = SET_SPI3_CPOL;
  	SPI_InitStructure.SPI_CPHA = SET_SPI3_CPHA;
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  	SPI_InitStructure.SPI_BaudRatePrescaler = SET_SPI3_HIGHSPEED;
  	SPI_InitStructure.SPI_FirstBit = SET_SPI3_FIRSTBIT;
  	SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI3,&SPI_InitStructure);
    SPI_Cmd(SPI3,ENABLE);
}  


/*
 * SPI3读写一个字节（发送完成后返回本次通讯读取的数据）
 *
 * @Param, TxData 待发送的数据
 *
 * @Return, 接收到的数据
 *
**/
u8 SPI3_ReadWrite(u8 TxData)
{
		while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);//等待发送区空
		SPI_I2S_SendData(SPI3, TxData); //通过外设 SPIx 发送一个 byte 数据
		while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);//等待接收完
		return SPI_I2S_ReceiveData(SPI3); //返回通过 SPIx 最近接收的数据			    
}

