/*
 *	Author:		GalaIO
 *	Date:			2015-7-26 10:12 AM
 *	Description:
 *			code a simulate IIC1 driver.
 *			
**/

#ifndef _BSP_IIC11_H_
#define _BSP_IIC11_H_

#include "bsp_user.h"
#include "bsp_gpio.h"
#include "bsp_sys.h"

#define IIC1_SDA_BIT		BIT9
#define IIC1_SDA_PIN		PIN9
#define IIC1_SCL_BIT		BIT8
#define IIC1_SCL_PIN		PIN8

//IO��������
#define IIC1_SDA_IN()  	{GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}
#define IIC1_SDA_OUT() 	{GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;}

//IO��������	 
#define IIC1_SCL    		PBout(IIC1_SCL_PIN) //SCL
#define IIC1_SDA    		PBout(IIC1_SDA_PIN) //SDA	 
#define IIC1_READ_SDA   PBin(IIC1_SDA_PIN)  //����SDA 

//IIC1���в�������
void IIC1_Init(void);                //��ʼ��IIC1��IO��				 
int IIC1_Start(void);				//����IIC1��ʼ�ź�
void IIC1_Stop(void);	  			//����IIC1ֹͣ�ź�
void IIC1_Send_Byte(u8 txd);			//IIC1����һ���ֽ�
u8 IIC1_Read_Byte(unsigned char ack);//IIC1��ȡһ���ֽ�
int IIC1_Wait_Ack(void); 				//IIC1�ȴ�ACK�ź�
void IIC1_Ack(void);					//IIC1����ACK�ź�
void IIC1_NAck(void);				//IIC1������ACK�ź�

void IIC1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC1_Read_One_Byte(u8 daddr,u8 addr);	 
unsigned char I2C1_Readkey(unsigned char I2C1_Addr);

unsigned char I2C1_ReadOneByte(unsigned char I2C1_Addr,unsigned char addr);
unsigned char IIC1writeByte(unsigned char dev, unsigned char reg, unsigned char data);
u8 IIC1writeBytes(u8 dev, u8 reg, u8 length, u8* data);
u8 IIC1writeBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
u8 IIC1writeBit(u8 dev,u8 reg,u8 bitNum,u8 data);
u8 IIC1readBytes(u8 dev, u8 reg, u8 length, u8 *data);

int I2C1Write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int I2C1Read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

#endif
