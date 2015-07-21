
#include "bsp_spi2.h"

/*
 * ��������SPI2_Init
 * ����  ��ʹ��
 * ����  ���� 
 * ���  ����
 * ����  ����
 */																						  
void SPI2_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

	//����SPIʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    //////������SPI���GPIO��ʼ��//////
    //SPI1ģ���Ӧ��SCK��MISO��MOSIΪAF����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ---------enc28j60 ����I/O��ʼ��----------------*/
	/* PB12-SPI2-NSS:ENC28J60_CS */ 											 // Ƭѡ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   // �������
    GPIO_Init(GPIOB, &GPIO_InitStructure);						  
	GPIO_SetBits(GPIOB, GPIO_Pin_12);										 // �Ȱ�Ƭѡ���ߣ������õ�ʱ��������

	/* PB13:ENC28J60_INT */						// �ж�����û�õ�

	/* PE1:ENC28J60_RST*/				      // ��λ�ƺ�����Ҳ����
 

	/* SPI2 ���� */ 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	/* Enable SPI2  */
    SPI_Cmd(SPI2, ENABLE); 
}

/*
 * ��������SPI2_ReadWrite
 * ����  ��SPI2��дһ�ֽ�����
 * ����  �� 
 * ���  ��
 * ����  ��
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

