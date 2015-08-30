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
*  Note:1.SPI �������ֹ�����ʽ SPI ģ��Ϊ�˺�����������ݽ������������蹤��Ҫ���������
*		��ͬ��ʱ�Ӽ��Ժ���λ���Խ������ã�ʱ�Ӽ��ԣ� CPOL���Դ���Э��û���ش��Ӱ�졣���
*		CPOL=0������ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ����� CPOL=1������ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�
*		ƽ��ʱ����λ�� CPHA���ܹ���������ѡ�����ֲ�ͬ�Ĵ���Э��֮һ�������ݴ��䡣��� CPHA=0��
*		�ڴ���ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ���������� CPHA=1���ڴ���ͬ��ʱ��
*		�ĵڶ��������أ��������½������ݱ������� SPI ��ģ�����֮ͨ�ŵ����豸ʱ����λ�ͼ���
*		Ӧ��һ�¡�
*  			2.������Ҫ��ȡ�ӻ���һ���ֽڣ��ͱ��뷢��һ�����ֽ��������ӻ��Ĵ��䡣�������SPI�����е�dummy����.
*				3.��Բ�ͬ��SPI���豸����Ҫ�Ķ�SPIʱ�Ӽ��ԣ�ʱ����λ�ȡ�
*******************************************************************************/
#include "bsp_spi1.h"
#include "bsp_gpio.h"
#include "bsp_user.h"
/*
 *��ʼ��SPI1.
**/
void SPI1_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure ;

	  /* GPIOA and GPIOB Periph clock enable */
		RCC_CMD(PA_PER|PB_PER);

    //����SPIʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

    //////������SPI���GPIO��ʼ��//////
    //SPI1ģ���Ӧ��SCK��MISO��MOSIΪAF����
		POUT_AF_INT(GPIOA,BIT5|BIT6|BIT7);
    GPIO_PinAFConfig(GPIOA,PIN5,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA,PIN6,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA,PIN7,GPIO_AF_SPI1);
	
    //PB6 pin:CS
		PBOUT_INT(BIT6);
		//pull up.
		PBOUT_SET(BIT6);

	
	  //////SPIģ������//////
		//SPI_Direction ���������� SPI ��ͨ�ŷ�ʽ������ѡ��Ϊ��˫����ȫ˫�����Լ����з��ʹ����շ�ʽ��
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		// SPI_Mode �������� SPI ������ģʽ��
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		//SPI_DataSiz Ϊ 8 λ���� 16 λ֡��ʽѡ����
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		//SPI_CPOL ��������ʱ�Ӽ��ԣ����ô���ͬ��ʱ�ӵĿ���״̬Ϊ�߻�͵�ƽ��
  	SPI_InitStructure.SPI_CPOL = SET_SPI1_CPOL;
		// SPI_CPHA ��������ʱ����λ��Ҳ����ѡ���ڴ���ͬ��ʱ�ӵĵڼ��������أ��������½������ݱ�����������Ϊ��һ�����ߵڶ��������زɼ���
  	SPI_InitStructure.SPI_CPHA = SET_SPI1_CPHA;
		//���� NSS �ź���Ӳ���� NSS �ܽţ�����������ơ�
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		//���� SPI ������Ԥ��ƵֵҲ���Ǿ��� SPI ��ʱ�ӵĲ������� 2 ��Ƶ�� 256 ��Ƶ 8 ����ѡֵ��������SPI�����ٶȺ�ʱ�Ӽ����
  	SPI_InitStructure.SPI_BaudRatePrescaler = SET_SPI1_BAUDPRE;
		//�������ݴ���˳���� MSB λ��ǰ���� LSB λ��ǰ����ͬSPI���豸Ҫ��ͬ��
  	SPI_InitStructure.SPI_FirstBit = SET_SPI1_FIRSTBIT;
		//�������� CRC У�����ʽ�����ͨ�ſɿ��ԣ����� 1 ���ɡ�
  	SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI1,&SPI_InitStructure);
		//���� SPI1��
    SPI_Cmd(SPI1,ENABLE);
		SPI1_ReadWrite(0xff);//��������	
}
 
  
/*
 *���SPI1 ��д�ٶ�
**/
void SPI1_HightSpeed(void) 
{
	  SPI_InitTypeDef SPI_InitStructure ;

    SPI_Cmd(SPI1,DISABLE);

    //////SPIģ������//////
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
 * SPI��дһ���ֽڣ�������ɺ󷵻ر���ͨѶ��ȡ�����ݣ�
 *
 * @Param, TxData �����͵�����
 *
 * @Return, ���յ�������
 *
**/
u8 SPI1_ReadWrite(u8 TxData)
{
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//�ȴ���������
		SPI_I2S_SendData(SPI1, TxData); //ͨ������ SPIx ����һ�� byte ����
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);//�ȴ�������
		return SPI_I2S_ReceiveData(SPI1); //����ͨ�� SPIx ������յ�����			    
}

