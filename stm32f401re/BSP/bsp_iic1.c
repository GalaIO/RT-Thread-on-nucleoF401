
#include "bsp_iic1.h"

#define delay_us	delayUS

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC1_Init(void)
*��������:		��ʼ��I2C1��Ӧ�Ľӿ����š�
*******************************************************************************/
void IIC1_Init(void)
{			
	RCC_CMD(PB_PER);   
	PBOUT_INT(IIC1_SDA_BIT|IIC1_SCL_BIT);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC1_Start(void)
*��������:		����IIC1��ʼ�ź�
*******************************************************************************/
int IIC1_Start(void)
{
	IIC1_SDA_OUT();     //sda�����
	IIC1_SDA=1;
	if(!IIC1_READ_SDA)return 0;	
	IIC1_SCL=1;
	delay_us(1);
 	IIC1_SDA=0;//START:when CLK is high,DATA change form high to low 
	if(IIC1_READ_SDA)return 0;
	delay_us(1);
	IIC1_SCL=0;//ǯסI2C1���ߣ�׼�����ͻ�������� 
	return 1;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC1_Stop(void)
*��������:	    //����IIC1ֹͣ�ź�
*******************************************************************************/	  
void IIC1_Stop(void)
{
	IIC1_SDA_OUT();//sda�����
	IIC1_SCL=0;
	IIC1_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(1);
	IIC1_SCL=1; 
	IIC1_SDA=1;//����I2C1���߽����ź�
	delay_us(1);							   	
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IIC1_Wait_Ack(void)
*��������:	    �ȴ�Ӧ���źŵ��� 
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
*******************************************************************************/
int IIC1_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC1_SDA_IN();      //SDA����Ϊ����  
	IIC1_SDA=1;
	delay_us(1);	   
	IIC1_SCL=1;
	delay_us(1);	 
	while(IIC1_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			IIC1_Stop();
			return 0;
		}
	  delay_us(1);
	}
	IIC1_SCL=0;//ʱ�����0 	   
	return 1;  
} 

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC1_Ack(void)
*��������:	    ����ACKӦ��
*******************************************************************************/
void IIC1_Ack(void)
{
	IIC1_SCL=0;
	IIC1_SDA_OUT();
	IIC1_SDA=0;
	delay_us(1);
	IIC1_SCL=1;
	delay_us(1);
	IIC1_SCL=0;
}
	
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC1_NAck(void)
*��������:	    ����NACKӦ��
*******************************************************************************/	    
void IIC1_NAck(void)
{
	IIC1_SCL=0;
	IIC1_SDA_OUT();
	IIC1_SDA=1;
	delay_us(1);
	IIC1_SCL=1;
	delay_us(1);
	IIC1_SCL=0;
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC1_Send_Byte(u8 txd)
*��������:	    IIC1����һ���ֽ�
*******************************************************************************/		  
void IIC1_Send_Byte(u8 txd)
{                        
    u8 t;   
	IIC1_SDA_OUT(); 	    
    IIC1_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC1_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(1);   
		IIC1_SCL=1;
		delay_us(1); 
		IIC1_SCL=0;	
		delay_us(1);
    }	 
} 	 
  
/**************************ʵ�ֺ���********************************************
*����ԭ��:		bool I2C1Write(uint8_t addr, uint8_t reg, uint8_t data)
*��������:		
*******************************************************************************/
int I2C1Write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
		int i;
    if (!IIC1_Start())
        return 1;
    IIC1_Send_Byte(addr << 1 );
    if (!IIC1_Wait_Ack()) {
        IIC1_Stop();
        return 1;
    }
    IIC1_Send_Byte(reg);
    IIC1_Wait_Ack();
		for (i = 0; i < len; i++) {
        IIC1_Send_Byte(data[i]);
        if (!IIC1_Wait_Ack()) {
            IIC1_Stop();
            return 0;
        }
    }
    IIC1_Stop();
    return 0;
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		bool I2C1Write(uint8_t addr, uint8_t reg, uint8_t data)
*��������:		
*******************************************************************************/
int I2C1Read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!IIC1_Start())
        return 1;
    IIC1_Send_Byte(addr << 1);
    if (!IIC1_Wait_Ack()) {
        IIC1_Stop();
        return 1;
    }
    IIC1_Send_Byte(reg);
    IIC1_Wait_Ack();
    IIC1_Start();
    IIC1_Send_Byte((addr << 1)+1);
    IIC1_Wait_Ack();
    while (len) {
        if (len == 1)
            *buf = IIC1_Read_Byte(0);
        else
            *buf = IIC1_Read_Byte(1);
        buf++;
        len--;
    }
    IIC1_Stop();
    return 0;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IIC1_Read_Byte(unsigned char ack)
*��������:	    //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK 
*******************************************************************************/  
u8 IIC1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC1_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC1_SCL=0; 
        delay_us(2);
				IIC1_SCL=1;
        receive<<=1;
        if(IIC1_READ_SDA)receive++;   
		delay_us(2); 
    }					 
    if (ack)
        IIC1_Ack(); //����ACK 
    else
        IIC1_NAck();//����nACK  
    return receive;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char I2C1_ReadOneByte(unsigned char I2C1_Addr,unsigned char addr)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	I2C1_Addr  Ŀ���豸��ַ
		addr	   �Ĵ�����ַ
����   ��������ֵ
*******************************************************************************/ 
unsigned char I2C1_ReadOneByte(unsigned char I2C1_Addr,unsigned char addr)
{
	unsigned char res=0;
	
	IIC1_Start();	
	IIC1_Send_Byte(I2C1_Addr);	   //����д����
	res++;
	IIC1_Wait_Ack();
	IIC1_Send_Byte(addr); res++;  //���͵�ַ
	IIC1_Wait_Ack();	  
	//IIC1_Stop();//����һ��ֹͣ����	
	IIC1_Start();
	IIC1_Send_Byte(I2C1_Addr+1); res++;          //�������ģʽ			   
	IIC1_Wait_Ack();
	res=IIC1_Read_Byte(0);	   
    IIC1_Stop();//����һ��ֹͣ����

	return res;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IIC1readBytes(u8 dev, u8 reg, u8 length, u8 *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ����� length��ֵ
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫ�����ֽ���
		*data  ���������ݽ�Ҫ��ŵ�ָ��
����   ���������ֽ�����
*******************************************************************************/ 
u8 IIC1readBytes(u8 dev, u8 reg, u8 length, u8 *data){
    u8 count = 0;
	
	IIC1_Start();
	IIC1_Send_Byte(dev);	   //����д����
	IIC1_Wait_Ack();
	IIC1_Send_Byte(reg);   //���͵�ַ
    IIC1_Wait_Ack();	  
	IIC1_Start();
	IIC1_Send_Byte(dev+1);  //�������ģʽ	
	IIC1_Wait_Ack();
	
    for(count=0;count<length;count++){
		 
		 if(count!=length-1)data[count]=IIC1_Read_Byte(1);  //��ACK�Ķ�����
		 	else  data[count]=IIC1_Read_Byte(0);	 //���һ���ֽ�NACK
	}
    IIC1_Stop();//����һ��ֹͣ����
    return 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IIC1writeBytes(u8 dev, u8 reg, u8 length, u8* data)
*��������:	    ������ֽ�д��ָ���豸 ָ���Ĵ���
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫд���ֽ���
		*data  ��Ҫд�����ݵ��׵�ַ
����   �����Ƿ�ɹ�
*******************************************************************************/ 
u8 IIC1writeBytes(u8 dev, u8 reg, u8 length, u8* data){
  
 	u8 count = 0;
	IIC1_Start();
	IIC1_Send_Byte(dev);	   //����д����
	IIC1_Wait_Ack();
	IIC1_Send_Byte(reg);   //���͵�ַ
    IIC1_Wait_Ack();	  
	for(count=0;count<length;count++){
		IIC1_Send_Byte(data[count]); 
		IIC1_Wait_Ack(); 
	 }
	IIC1_Stop();//����һ��ֹͣ����

    return 0; //status == 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IIC1readByte(u8 dev, u8 reg, u8 *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		*data  ���������ݽ�Ҫ��ŵĵ�ַ
����   1
*******************************************************************************/ 
u8 IIC1readByte(u8 dev, u8 reg, u8 *data){
	*data=I2C1_ReadOneByte(dev, reg);
    return 1;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IIC1writeByte(unsigned char dev, unsigned char reg, unsigned char data)
*��������:	    д��ָ���豸 ָ���Ĵ���һ���ֽ�
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		data  ��Ҫд����ֽ�
����   1
*******************************************************************************/ 
unsigned char IIC1writeByte(unsigned char dev, unsigned char reg, unsigned char data){
    return IIC1writeBytes(dev, reg, 1, &data);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IIC1writeBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �еĶ��λ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		bitStart  Ŀ���ֽڵ���ʼλ
		length   λ����
		data    ��Ÿı�Ŀ���ֽ�λ��ֵ
����   �ɹ� Ϊ1 
 		ʧ��Ϊ0
*******************************************************************************/ 
u8 IIC1writeBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{

    u8 b;
    if (IIC1readByte(dev, reg, &b) != 0) {
        u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return IIC1writeByte(dev, reg, b);
    } else {
        return 0;
    }
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IIC1writeBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		bitNum  Ҫ�޸�Ŀ���ֽڵ�bitNumλ
		data  Ϊ0 ʱ��Ŀ��λ������0 ���򽫱���λ
����   �ɹ� Ϊ1 
 		ʧ��Ϊ0
*******************************************************************************/ 
u8 IIC1writeBit(u8 dev, u8 reg, u8 bitNum, u8 data){
    u8 b;
    IIC1readByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return IIC1writeByte(dev, reg, b);
}

