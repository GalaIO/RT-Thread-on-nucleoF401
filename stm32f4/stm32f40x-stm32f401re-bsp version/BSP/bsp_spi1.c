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
#include "bsp_spi1.h"

/* Private macro -------------------------------z------------------------------*/
/*//进行移植时,需要修改
#define	 SD_CS_PORT   GPIOA
#define	 SD_CS_PIN	  GPIO_Pin_4
#define	 SD_SPI_NUM	  SPI1
#define	 SPI_PORT	  GPIOA
#define	 SPI_MISO	  GPIO_Pin_6
#define	 SPI_PINS	  (GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)  */
void SPI1_Init(void)//配置SPI接口	
{
    SPI_InitTypeDef SPI_InitStructure ;
    GPIO_InitTypeDef GPIO_InitStructure ;

	/* GPIOA and GPIOB Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    //启动SPI时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

    //////下面是SPI相关GPIO初始化//////
    //SPI1模块对应的SCK、MISO、MOSI为AF引脚
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP ;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

/*	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //上拉输入
    GPIO_Init(GPIOA,&GPIO_InitStructure);*/	
    
    
    //PB6 pin:SD_CS
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ;
    GPIO_Init(GPIOA,&GPIO_InitStructure); 

	GPIO_SetBits(GPIOA,GPIO_Pin_4);


   // SPI_Cmd(SPI1,DISABLE);

	//////SPI模块配置//////

  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  	SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI1,&SPI_InitStructure);
    SPI_Cmd(SPI1,ENABLE);
}
 
  
//进行移植时,需要修改
void SPI1_HightSpeed(void)  //提高SD卡的读写速度
{
	SPI_InitTypeDef SPI_InitStructure ;

    //SPI_Cmd(SD_SPI_NUM,DISABLE);

    //////SPI模块配置//////
  	/* SPI1 Config */
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//4
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  	SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI1,&SPI_InitStructure);
    SPI_Cmd(SPI1,ENABLE);
}  


/*******************************************************************************
* Function Name  : SPI1_ReadWriteByte
* Description    : SPI读写一个字节（发送完成后返回本次通讯读取的数据）
* Input          : u8 TxData 待发送的数
* Output         : None
* Return         : u8 RxData 收到的数
*******************************************************************************/
//进行移植时,需要修改
u8 SPI1_ReadWrite(u8 TxData)//在高速SD卡的驱动程序中用到
{
	while((SPI1->SR&1<<1)==0);//等待发送区空				  
	SPI1->DR=TxData;	 	  //发送一个byte   
	while((SPI1->SR&1<<0)==0);//等待接收完一个byte  
	return SPI1->DR;          //返回收到的数据				    
}

