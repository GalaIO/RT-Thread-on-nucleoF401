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

	  //����SPIʱ��
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3, ENABLE);
    //////������SPI���GPIO��ʼ��//////
		RCC_CMD(PB_PER);
    //SPI1ģ���Ӧ��SCK��MISO��MOSIΪAF����
		POUT_AF_INT(GPIOB,BIT3|BIT4|BIT5);
    GPIO_PinAFConfig(GPIOB,PIN3,GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOB,PIN4,GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOB,PIN5,GPIO_AF_SPI3);
	
	  /* PB10-SPI3-NSS:ENC28J60_CS */ 											 // Ƭѡ
    PBOUT_INT(BIT10);
		//pull up
		PBOUT_SET(BIT10);
	
	  //////SPIģ������//////
		//SPI_Direction ���������� SPI ��ͨ�ŷ�ʽ������ѡ��Ϊ��˫����ȫ˫�����Լ����з��ʹ����շ�ʽ��
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		// SPI_Mode �������� SPI ������ģʽ��
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		//SPI_DataSiz Ϊ 8 λ���� 16 λ֡��ʽѡ����
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		//SPI_CPOL ��������ʱ�Ӽ��ԣ����ô���ͬ��ʱ�ӵĿ���״̬Ϊ�߻�͵�ƽ��
  	SPI_InitStructure.SPI_CPOL = SET_SPI3_CPOL;
		// SPI_CPHA ��������ʱ����λ��Ҳ����ѡ���ڴ���ͬ��ʱ�ӵĵڼ��������أ��������½������ݱ�����������Ϊ��һ�����ߵڶ��������زɼ���
  	SPI_InitStructure.SPI_CPHA = SET_SPI3_CPHA;
		//���� NSS �ź���Ӳ���� NSS �ܽţ�����������ơ�
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		//���� SPI ������Ԥ��ƵֵҲ���Ǿ��� SPI ��ʱ�ӵĲ������� 2 ��Ƶ�� 256 ��Ƶ 8 ����ѡֵ��������SPI�����ٶȺ�ʱ�Ӽ����
  	SPI_InitStructure.SPI_BaudRatePrescaler = SET_SPI3_BAUDPRE;
		//�������ݴ���˳���� MSB λ��ǰ���� LSB λ��ǰ����ͬSPI���豸Ҫ��ͬ��
  	SPI_InitStructure.SPI_FirstBit = SET_SPI3_FIRSTBIT;
		//�������� CRC У�����ʽ�����ͨ�ſɿ��ԣ����� 1 ���ɡ�
  	SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI3,&SPI_InitStructure);
		//���� SPI1��
    SPI_Cmd(SPI3,ENABLE);
		SPI3_ReadWrite(0xff);//��������	
}

  
/*
 *���SPI3 ��д�ٶ�
**/
void SPI3_HightSpeed(void) 
{
	  SPI_InitTypeDef SPI_InitStructure ;

    SPI_Cmd(SPI3,DISABLE);

    //////SPIģ������//////
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
 * SPI3��дһ���ֽڣ�������ɺ󷵻ر���ͨѶ��ȡ�����ݣ�
 *
 * @Param, TxData �����͵�����
 *
 * @Return, ���յ�������
 *
**/
u8 SPI3_ReadWrite(u8 TxData)
{
		while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);//�ȴ���������
		SPI_I2S_SendData(SPI3, TxData); //ͨ������ SPIx ����һ�� byte ����
		while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);//�ȴ�������
		return SPI_I2S_ReceiveData(SPI3); //����ͨ�� SPIx ������յ�����			    
}

