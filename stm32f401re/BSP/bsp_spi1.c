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
*  Note:1.SPI 总线四种工作方式 SPI 模块为了和外设进行数据交换，根据外设工作要求，其输出串
*		行同步时钟极性和相位可以进行配置，时钟极性（ CPOL）对传输协议没有重大的影响。如果
*		CPOL=0，串行同步时钟的空闲状态为低电平；如果 CPOL=1，串行同步时钟的空闲状态为高电
*		平。时钟相位（ CPHA）能够配置用于选择两种不同的传输协议之一进行数据传输。如果 CPHA=0，
*		在串行同步时钟的第一个跳变沿（上升或下降）数据被采样；如果 CPHA=1，在串行同步时钟
*		的第二个跳变沿（上升或下降）数据被采样。 SPI 主模块和与之通信的外设备时钟相位和极性
*		应该一致。
*  			2.若主机要读取从机的一个字节，就必须发送一个空字节来引发从机的传输。这个就是SPI操作中的dummy操作.
*				3.面对不同的SPI子设备，需要改动SPI时钟极性，时钟相位等。
*******************************************************************************/
#include "bsp_spi1.h"
#include "bsp_gpio.h"
#include "bsp_user.h"
/*
 *初始化SPI1.
**/
void SPI1_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure ;

	  /* GPIOA and GPIOB Periph clock enable */
		RCC_CMD(PA_PER|PB_PER);

    //启动SPI时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

    //////下面是SPI相关GPIO初始化//////
    //SPI1模块对应的SCK、MISO、MOSI为AF引脚
		POUT_AF_INT(GPIOA,BIT5|BIT6|BIT7);
    GPIO_PinAFConfig(GPIOA,PIN5,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA,PIN6,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA,PIN7,GPIO_AF_SPI1);
	
    //PB6 pin:CS
		PBOUT_INT(BIT6);
		//pull up.
		PBOUT_SET(BIT6);

	
	  //////SPI模块配置//////
		//SPI_Direction 是用来设置 SPI 的通信方式，可以选择为半双工，全双工，以及串行发和串行收方式，
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		// SPI_Mode 用来设置 SPI 的主从模式，
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		//SPI_DataSiz 为 8 位还是 16 位帧格式选择项
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		//SPI_CPOL 用来设置时钟极性，设置串行同步时钟的空闲状态为高或低电平。
  	SPI_InitStructure.SPI_CPOL = SET_SPI1_CPOL;
		// SPI_CPHA 用来设置时钟相位，也就是选择在串行同步时钟的第几个跳变沿（上升或下降）数据被采样，可以为第一个或者第二个条边沿采集。
  	SPI_InitStructure.SPI_CPHA = SET_SPI1_CPHA;
		//设置 NSS 信号由硬件（ NSS 管脚）还是软件控制。
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		//设置 SPI 波特率预分频值也就是决定 SPI 的时钟的参数，从 2 分频到 256 分频 8 个可选值，决定了SPI传输速度和时钟间隔。
  	SPI_InitStructure.SPI_BaudRatePrescaler = SET_SPI1_BAUDPRE;
		//设置数据传输顺序是 MSB 位在前还是 LSB 位在前，不同SPI子设备要求不同。
  	SPI_InitStructure.SPI_FirstBit = SET_SPI1_FIRSTBIT;
		//用来设置 CRC 校验多项式，提高通信可靠性，大于 1 即可。
  	SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI1,&SPI_InitStructure);
		//启动 SPI1。
    SPI_Cmd(SPI1,ENABLE);
		SPI1_ReadWrite(0xff);//启动传输	
}
 
  
/*
 *提高SPI1 读写速度
**/
void SPI1_HightSpeed(void) 
{
	  SPI_InitTypeDef SPI_InitStructure ;

    SPI_Cmd(SPI1,DISABLE);

    //////SPI模块配置//////
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  	SPI_InitStructure.SPI_CPOL = SET_SPI1_CPOL;
  	SPI_InitStructure.SPI_CPHA = SET_SPI1_CPHA;
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  	SPI_InitStructure.SPI_BaudRatePrescaler = SET_SPI1_HIGHSPEED;
  	SPI_InitStructure.SPI_FirstBit = SET_SPI1_FIRSTBIT;
  	SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI1,&SPI_InitStructure);
    SPI_Cmd(SPI1,ENABLE);
}  


/*
 * SPI读写一个字节（发送完成后返回本次通讯读取的数据）
 *
 * @Param, TxData 待发送的数据
 *
 * @Return, 接收到的数据
 *
**/
u8 SPI1_ReadWrite(u8 TxData)
{
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//等待发送区空
		SPI_I2S_SendData(SPI1, TxData); //通过外设 SPIx 发送一个 byte 数据
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);//等待接收完
		return SPI_I2S_ReceiveData(SPI1); //返回通过 SPIx 最近接收的数据			    
}

