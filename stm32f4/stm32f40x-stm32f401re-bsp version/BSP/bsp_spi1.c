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
/*//������ֲʱ,��Ҫ�޸�
#define	 SD_CS_PORT   GPIOA
#define	 SD_CS_PIN	  GPIO_Pin_4
#define	 SD_SPI_NUM	  SPI1
#define	 SPI_PORT	  GPIOA
#define	 SPI_MISO	  GPIO_Pin_6
#define	 SPI_PINS	  (GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)  */
void SPI1_Init(void)//����SPI�ӿ�	
{
    SPI_InitTypeDef SPI_InitStructure ;
    GPIO_InitTypeDef GPIO_InitStructure ;

	/* GPIOA and GPIOB Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    //����SPIʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

    //////������SPI���GPIO��ʼ��//////
    //SPI1ģ���Ӧ��SCK��MISO��MOSIΪAF����
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP ;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

/*	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //��������
    GPIO_Init(GPIOA,&GPIO_InitStructure);*/	
    
    
    //PB6 pin:SD_CS
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ;
    GPIO_Init(GPIOA,&GPIO_InitStructure); 

	GPIO_SetBits(GPIOA,GPIO_Pin_4);


   // SPI_Cmd(SPI1,DISABLE);

	//////SPIģ������//////

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
 
  
//������ֲʱ,��Ҫ�޸�
void SPI1_HightSpeed(void)  //���SD���Ķ�д�ٶ�
{
	SPI_InitTypeDef SPI_InitStructure ;

    //SPI_Cmd(SD_SPI_NUM,DISABLE);

    //////SPIģ������//////
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
* Description    : SPI��дһ���ֽڣ�������ɺ󷵻ر���ͨѶ��ȡ�����ݣ�
* Input          : u8 TxData �����͵���
* Output         : None
* Return         : u8 RxData �յ�����
*******************************************************************************/
//������ֲʱ,��Ҫ�޸�
u8 SPI1_ReadWrite(u8 TxData)//�ڸ���SD���������������õ�
{
	while((SPI1->SR&1<<1)==0);//�ȴ���������				  
	SPI1->DR=TxData;	 	  //����һ��byte   
	while((SPI1->SR&1<<0)==0);//�ȴ�������һ��byte  
	return SPI1->DR;          //�����յ�������				    
}

