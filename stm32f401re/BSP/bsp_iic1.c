
#include "bsp_iic1.h"

#define delay_us	delayUS

/**************************实现函数********************************************
*函数原型:		void IIC1_Init(void)
*功　　能:		初始化I2C1对应的接口引脚。
*******************************************************************************/
void IIC1_Init(void)
{			
	RCC_CMD(PB_PER);   
	PBOUT_INT(IIC1_SDA_BIT|IIC1_SCL_BIT);
}

/**************************实现函数********************************************
*函数原型:		void IIC1_Start(void)
*功　　能:		产生IIC1起始信号
*******************************************************************************/
int IIC1_Start(void)
{
	IIC1_SDA_OUT();     //sda线输出
	IIC1_SDA=1;
	if(!IIC1_READ_SDA)return 0;	
	IIC1_SCL=1;
	delay_us(1);
 	IIC1_SDA=0;//START:when CLK is high,DATA change form high to low 
	if(IIC1_READ_SDA)return 0;
	delay_us(1);
	IIC1_SCL=0;//钳住I2C1总线，准备发送或接收数据 
	return 1;
}

/**************************实现函数********************************************
*函数原型:		void IIC1_Stop(void)
*功　　能:	    //产生IIC1停止信号
*******************************************************************************/	  
void IIC1_Stop(void)
{
	IIC1_SDA_OUT();//sda线输出
	IIC1_SCL=0;
	IIC1_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(1);
	IIC1_SCL=1; 
	IIC1_SDA=1;//发送I2C1总线结束信号
	delay_us(1);							   	
}

/**************************实现函数********************************************
*函数原型:		u8 IIC1_Wait_Ack(void)
*功　　能:	    等待应答信号到来 
//返回值：1，接收应答失败
//        0，接收应答成功
*******************************************************************************/
int IIC1_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC1_SDA_IN();      //SDA设置为输入  
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
	IIC1_SCL=0;//时钟输出0 	   
	return 1;  
} 

/**************************实现函数********************************************
*函数原型:		void IIC1_Ack(void)
*功　　能:	    产生ACK应答
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
	
/**************************实现函数********************************************
*函数原型:		void IIC1_NAck(void)
*功　　能:	    产生NACK应答
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
/**************************实现函数********************************************
*函数原型:		void IIC1_Send_Byte(u8 txd)
*功　　能:	    IIC1发送一个字节
*******************************************************************************/		  
void IIC1_Send_Byte(u8 txd)
{                        
    u8 t;   
	IIC1_SDA_OUT(); 	    
    IIC1_SCL=0;//拉低时钟开始数据传输
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
  
/**************************实现函数********************************************
*函数原型:		bool I2C1Write(uint8_t addr, uint8_t reg, uint8_t data)
*功　　能:		
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
/**************************实现函数********************************************
*函数原型:		bool I2C1Write(uint8_t addr, uint8_t reg, uint8_t data)
*功　　能:		
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


/**************************实现函数********************************************
*函数原型:		u8 IIC1_Read_Byte(unsigned char ack)
*功　　能:	    //读1个字节，ack=1时，发送ACK，ack=0，发送nACK 
*******************************************************************************/  
u8 IIC1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC1_SDA_IN();//SDA设置为输入
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
        IIC1_Ack(); //发送ACK 
    else
        IIC1_NAck();//发送nACK  
    return receive;
}

/**************************实现函数********************************************
*函数原型:		unsigned char I2C1_ReadOneByte(unsigned char I2C1_Addr,unsigned char addr)
*功　　能:	    读取指定设备 指定寄存器的一个值
输入	I2C1_Addr  目标设备地址
		addr	   寄存器地址
返回   读出来的值
*******************************************************************************/ 
unsigned char I2C1_ReadOneByte(unsigned char I2C1_Addr,unsigned char addr)
{
	unsigned char res=0;
	
	IIC1_Start();	
	IIC1_Send_Byte(I2C1_Addr);	   //发送写命令
	res++;
	IIC1_Wait_Ack();
	IIC1_Send_Byte(addr); res++;  //发送地址
	IIC1_Wait_Ack();	  
	//IIC1_Stop();//产生一个停止条件	
	IIC1_Start();
	IIC1_Send_Byte(I2C1_Addr+1); res++;          //进入接收模式			   
	IIC1_Wait_Ack();
	res=IIC1_Read_Byte(0);	   
    IIC1_Stop();//产生一个停止条件

	return res;
}


/**************************实现函数********************************************
*函数原型:		u8 IIC1readBytes(u8 dev, u8 reg, u8 length, u8 *data)
*功　　能:	    读取指定设备 指定寄存器的 length个值
输入	dev  目标设备地址
		reg	  寄存器地址
		length 要读的字节数
		*data  读出的数据将要存放的指针
返回   读出来的字节数量
*******************************************************************************/ 
u8 IIC1readBytes(u8 dev, u8 reg, u8 length, u8 *data){
    u8 count = 0;
	
	IIC1_Start();
	IIC1_Send_Byte(dev);	   //发送写命令
	IIC1_Wait_Ack();
	IIC1_Send_Byte(reg);   //发送地址
    IIC1_Wait_Ack();	  
	IIC1_Start();
	IIC1_Send_Byte(dev+1);  //进入接收模式	
	IIC1_Wait_Ack();
	
    for(count=0;count<length;count++){
		 
		 if(count!=length-1)data[count]=IIC1_Read_Byte(1);  //带ACK的读数据
		 	else  data[count]=IIC1_Read_Byte(0);	 //最后一个字节NACK
	}
    IIC1_Stop();//产生一个停止条件
    return 0;
}

/**************************实现函数********************************************
*函数原型:		u8 IIC1writeBytes(u8 dev, u8 reg, u8 length, u8* data)
*功　　能:	    将多个字节写入指定设备 指定寄存器
输入	dev  目标设备地址
		reg	  寄存器地址
		length 要写的字节数
		*data  将要写的数据的首地址
返回   返回是否成功
*******************************************************************************/ 
u8 IIC1writeBytes(u8 dev, u8 reg, u8 length, u8* data){
  
 	u8 count = 0;
	IIC1_Start();
	IIC1_Send_Byte(dev);	   //发送写命令
	IIC1_Wait_Ack();
	IIC1_Send_Byte(reg);   //发送地址
    IIC1_Wait_Ack();	  
	for(count=0;count<length;count++){
		IIC1_Send_Byte(data[count]); 
		IIC1_Wait_Ack(); 
	 }
	IIC1_Stop();//产生一个停止条件

    return 0; //status == 0;
}

/**************************实现函数********************************************
*函数原型:		u8 IIC1readByte(u8 dev, u8 reg, u8 *data)
*功　　能:	    读取指定设备 指定寄存器的一个值
输入	dev  目标设备地址
		reg	   寄存器地址
		*data  读出的数据将要存放的地址
返回   1
*******************************************************************************/ 
u8 IIC1readByte(u8 dev, u8 reg, u8 *data){
	*data=I2C1_ReadOneByte(dev, reg);
    return 1;
}

/**************************实现函数********************************************
*函数原型:		unsigned char IIC1writeByte(unsigned char dev, unsigned char reg, unsigned char data)
*功　　能:	    写入指定设备 指定寄存器一个字节
输入	dev  目标设备地址
		reg	   寄存器地址
		data  将要写入的字节
返回   1
*******************************************************************************/ 
unsigned char IIC1writeByte(unsigned char dev, unsigned char reg, unsigned char data){
    return IIC1writeBytes(dev, reg, 1, &data);
}

/**************************实现函数********************************************
*函数原型:		u8 IIC1writeBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的多个位
输入	dev  目标设备地址
		reg	   寄存器地址
		bitStart  目标字节的起始位
		length   位长度
		data    存放改变目标字节位的值
返回   成功 为1 
 		失败为0
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

/**************************实现函数********************************************
*函数原型:		u8 IIC1writeBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的1个位
输入	dev  目标设备地址
		reg	   寄存器地址
		bitNum  要修改目标字节的bitNum位
		data  为0 时，目标位将被清0 否则将被置位
返回   成功 为1 
 		失败为0
*******************************************************************************/ 
u8 IIC1writeBit(u8 dev, u8 reg, u8 bitNum, u8 data){
    u8 b;
    IIC1readByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return IIC1writeByte(dev, reg, b);
}

