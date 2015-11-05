/*
 *	Author:		GalaIO
 *	Date:			2015-9-4 19:13 PM
 *	Description:
 *		code the ENC28J60 with SPI3.
 *                      Pin assignment:注意该引脚分布是ENC28J60的晶振向上。
 *
 *             ----------------------------------------------------
 *             | PIN12-ENC-CLK-clkout  |   PIN9-ENC-INT-PA8       |
 *             | PIN8-ENC-WOL-wakeup	 |   PIN7-SPI3-MISO-PB4     |
 *             | PIN6-SPI3-MOSI-PB5 	 |   PIN5-SPI3-SCK-PB3      |
 *             | PIN4-SPI3-CS-PB10     |   PIN3-ENC-RST-VCC-3.3v  |
 *             | PIN2-VCC-3.3V    		 |   PIN1-GND               |
 *             ----------------------------------------------------
 *
**/
#include "bsp_enc28j60.h"

#include "bsp_exti.h"
#include "bsp_sys.h"
#include "bsp_spi3.h"
#include "bsp_gpio.h"

#define MAX_ADDR_LEN    6

#define SPI3_CSL() 	PBOUT_CLR(BIT10)
#define SPI3_CSH() 	PBOUT_SET(BIT10)

static const u8 enc29j60_mac[MAX_ADDR_LEN] = {0x82,0xed,0xee,0x82,0xff,0xfe};

static unsigned char ENC28J60BANK;
static unsigned int  NextPacketPtr;
//读取ENC28J60寄存器(带操作码) 
//op：操作码
//addr:寄存器地址/参数
//返回值:读到的数据
u8 ENC28J60_Read_Op(u8 op,u8 addr)
{
	u8 dat=0;	 
	SPI3_CSL();	 
	dat=op|(addr&ADDR_MASK);
	SPI3_ReadWrite(dat);
	dat=SPI3_ReadWrite(0xFF);
	//如果是读取MAC/MII寄存器,则第二次读到的数据才是正确的,见手册29页
 	if(addr&0x80)dat=SPI3_ReadWrite(0xFF);
	SPI3_CSH();
	return dat;
}
//读取ENC28J60寄存器(带操作码) 
//op：操作码
//addr:寄存器地址
//data:参数
void ENC28J60_Write_Op(u8 op,u8 addr,u8 data)
{
	u8 dat = 0;	   
	SPI3_CSL();				   
	dat=op|(addr&ADDR_MASK);
	SPI3_ReadWrite(dat);	  
	SPI3_ReadWrite(data);
	SPI3_CSH();
}
//读取ENC28J60接收缓存数据
//len:要读取的数据长度
//data:输出数据缓存区(末尾自动添加结束符)
void ENC28J60_Read_Buf(u32 len,u8* data)
{
	SPI3_CSL();			 
	SPI3_ReadWrite(ENC28J60_READ_BUF_MEM);
	while(len)
	{
		len--;			  
		*data=(u8)SPI3_ReadWrite(0);
		data++;
	}
	*data='\0';
	SPI3_CSH();
}
//向ENC28J60写发送缓存数据
//len:要写入的数据长度
//data:数据缓存区 
void ENC28J60_Write_Buf(u32 len,u8* data)
{
	SPI3_CSL();				   
	SPI3_ReadWrite(ENC28J60_WRITE_BUF_MEM);		 
	while(len)
	{
		len--;
		SPI3_ReadWrite(*data);
		data++;
	}
	SPI3_CSH();
}
//设置ENC28J60寄存器Bank
//ban:要设置的bank
void ENC28J60_Set_Bank(u8 bank)
{								    
	if((bank&BANK_MASK)!=ENC28J60BANK)//和当前bank不一致的时候,才设置
	{				  
		ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR,ECON1,(ECON1_BSEL1|ECON1_BSEL0));
		ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET,ECON1,(bank&BANK_MASK)>>5);
		ENC28J60BANK=(bank&BANK_MASK);
	}
}
//读取ENC28J60指定寄存器 
//addr:寄存器地址
//返回值:读到的数据
u8 ENC28J60_Read(u8 addr)
{						  
	ENC28J60_Set_Bank(addr);//设置BANK		 
	return ENC28J60_Read_Op(ENC28J60_READ_CTRL_REG,addr);
}
//向ENC28J60指定寄存器写数据
//addr:寄存器地址
//data:要写入的数据		 
void ENC28J60_Write(u8 addr,u8 data)
{					  
	ENC28J60_Set_Bank(addr);		 
	ENC28J60_Write_Op(ENC28J60_WRITE_CTRL_REG,addr,data);
}
//向ENC28J60的PHY寄存器写入数据
//addr:寄存器地址
//data:要写入的数据		 
void ENC28J60_PHY_Write(u8 addr,u32 data)
{
	u16 retry=0;
	ENC28J60_Write(MIREGADR,addr);	//设置PHY寄存器地址
	ENC28J60_Write(MIWRL,data);		//写入数据
	ENC28J60_Write(MIWRH,data>>8);		   
	while((ENC28J60_Read(MISTAT)&MISTAT_BUSY)&&retry<0XFFF)retry++;//等待写入PHY结束		  
}

// read upper 8 bits
rt_uint16_t ENC28J60_PHY_Read(u8 address)
{
	// Set the right address and start the register read operation
	ENC28J60_Write(MIREGADR, address);
	ENC28J60_Write(MICMD, MICMD_MIIRD);

	delayUS(15);

	// wait until the PHY read completes
	while(ENC28J60_Read(MISTAT) & MISTAT_BUSY);

	// reset reading bit
	ENC28J60_Write(MICMD, 0x00);

	return (ENC28J60_Read(MIRDH));
}
/*
 * Access the PHY to determine link status
 */
static u8 ENC28J60_Check_Link_Status()
{
	rt_uint16_t reg;
	//int duplex;

	reg = ENC28J60_PHY_Read(PHSTAT2);
	//duplex = reg & PHSTAT2_DPXSTAT;

	if (reg & PHSTAT2_LSTAT){
	    /* on */
      return 1;
	}else{
	    /* off */
      return 0;
	}
}
//初始化ENC28J60
//macaddr:MAC地址
//返回值:0,初始化成功;
//       1,初始化失败;
u8 ENC28J60_Init()
{		
	u16 retry=0;		  
	
	//init spi3
	SPI3_Init();
	
//	//init INT pin.
//	//PA8
//	RCC_CMD(PA_PER);
//	PAIN_INT(BIT8);
//	SYSCFG_ENABLE();
//	PAEXTI_INT(PIN8,EXTI_F);
//	//GENERAL_NVIC_CMD(EXTI9_5_IRQn);
//	NVIC_CMD(EXTI9_5_IRQn,TOP_PREEMPTION,TOP_PRIORITY);
	
	ENC28J60_Write_Op(ENC28J60_SOFT_RESET,0,ENC28J60_SOFT_RESET);//软件复位
	while(!(ENC28J60_Read(ESTAT)&ESTAT_CLKRDY)&&retry<500)//等待时钟稳定
	{
		retry++;
		delayMS(1);
	};
	if(retry>=500)return 1;//ENC28J60初始化失败
	// do bank 0 stuff
	// initialize receive buffer
	// 16-bit transfers,must write low byte first
	// set receive buffer start address	   设置接收缓冲区地址  8K字节容量
	NextPacketPtr=RXSTART_INIT;
	// Rx start
	//接收缓冲器由一个硬件管理的循环FIFO 缓冲器构成。
	//寄存器对ERXSTH:ERXSTL 和ERXNDH:ERXNDL 作
	//为指针，定义缓冲器的容量和其在存储器中的位置。
	//ERXST和ERXND指向的字节均包含在FIFO缓冲器内。
	//当从以太网接口接收数据字节时，这些字节被顺序写入
	//接收缓冲器。 但是当写入由ERXND 指向的存储单元
	//后，硬件会自动将接收的下一字节写入由ERXST 指向
	//的存储单元。 因此接收硬件将不会写入FIFO 以外的单
	//元。
	//设置接收起始字节
	ENC28J60_Write(ERXSTL,RXSTART_INIT&0xFF);	
	ENC28J60_Write(ERXSTH,RXSTART_INIT>>8);	  
	//ERXWRPTH:ERXWRPTL 寄存器定义硬件向FIFO 中
	//的哪个位置写入其接收到的字节。 指针是只读的，在成
	//功接收到一个数据包后，硬件会自动更新指针。 指针可
	//用于判断FIFO 内剩余空间的大小  8K-1500。 
	//设置接收读指针字节
	ENC28J60_Write(ERXRDPTL,RXSTART_INIT&0xFF);
	ENC28J60_Write(ERXRDPTH,RXSTART_INIT>>8);
	//设置接收结束字节
	ENC28J60_Write(ERXNDL,RXSTOP_INIT&0xFF);
	ENC28J60_Write(ERXNDH,RXSTOP_INIT>>8);
	//设置发送起始字节
	ENC28J60_Write(ETXSTL,TXSTART_INIT&0xFF);
	ENC28J60_Write(ETXSTH,TXSTART_INIT>>8);
	//设置发送结束字节
	ENC28J60_Write(ETXNDL,TXSTOP_INIT&0xFF);
	ENC28J60_Write(ETXNDH,TXSTOP_INIT>>8);
	// do bank 1 stuff,packet filter:
	// For broadcast packets we allow only ARP packtets
	// All other packets should be unicast only for our mac (MAADR)
	//
	// The pattern to match on is therefore
	// Type     ETH.DST
	// ARP      BROADCAST
	// 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
	// in binary these poitions are:11 0000 0011 1111
	// This is hex 303F->EPMM0=0x3f,EPMM1=0x30
	//接收过滤器
	//UCEN：单播过滤器使能位
	//当ANDOR = 1 时：
	//1 = 目标地址与本地MAC 地址不匹配的数据包将被丢弃
	//0 = 禁止过滤器
	//当ANDOR = 0 时：
	//1 = 目标地址与本地MAC 地址匹配的数据包会被接受
	//0 = 禁止过滤器
	//CRCEN：后过滤器CRC 校验使能位
	//1 = 所有CRC 无效的数据包都将被丢弃
	//0 = 不考虑CRC 是否有效
	//PMEN：格式匹配过滤器使能位
	//当ANDOR = 1 时：
	//1 = 数据包必须符合格式匹配条件，否则将被丢弃
	//0 = 禁止过滤器
	//当ANDOR = 0 时：
	//1 = 符合格式匹配条件的数据包将被接受
	//0 = 禁止过滤器
	ENC28J60_Write(ERXFCON,ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN);
	ENC28J60_Write(EPMM0,0x3f);
	ENC28J60_Write(EPMM1,0x30);
	ENC28J60_Write(EPMCSL,0xf9);
	ENC28J60_Write(EPMCSH,0xf7);
	// do bank 2 stuff
	// enable MAC receive
	//bit 0 MARXEN：MAC 接收使能位
	//1 = 允许MAC 接收数据包
	//0 = 禁止数据包接收
	//bit 3 TXPAUS：暂停控制帧发送使能位
	//1 = 允许MAC 发送暂停控制帧（用于全双工模式下的流量控制）
	//0 = 禁止暂停帧发送
	//bit 2 RXPAUS：暂停控制帧接收使能位
	//1 = 当接收到暂停控制帧时，禁止发送（正常操作）
	//0 = 忽略接收到的暂停控制帧
	ENC28J60_Write(MACON1,MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
	// bring MAC out of reset
	//将MACON2 中的MARST 位清零，使MAC 退出复位状态。
	ENC28J60_Write(MACON2,0x00);
	// enable automatic padding to 60bytes and CRC operations
	//bit 7-5 PADCFG2:PACDFG0：自动填充和CRC 配置位
	//111 = 用0 填充所有短帧至64 字节长，并追加一个有效的CRC
	//110 = 不自动填充短帧
	//101 = MAC 自动检测具有8100h 类型字段的VLAN 协议帧，并自动填充到64 字节长。如果不
	//是VLAN 帧，则填充至60 字节长。填充后还要追加一个有效的CRC
	//100 = 不自动填充短帧
	//011 = 用0 填充所有短帧至64 字节长，并追加一个有效的CRC
	//010 = 不自动填充短帧
	//001 = 用0 填充所有短帧至60 字节长，并追加一个有效的CRC
	//000 = 不自动填充短帧
	//bit 4 TXCRCEN：发送CRC 使能位
	//1 = 不管PADCFG如何，MAC都会在发送帧的末尾追加一个有效的CRC。 如果PADCFG规定要
	//追加有效的CRC，则必须将TXCRCEN 置1。
	//0 = MAC不会追加CRC。 检查最后4 个字节，如果不是有效的CRC 则报告给发送状态向量。
	//bit 0 FULDPX：MAC 全双工使能位
	//1 = MAC工作在全双工模式下。 PHCON1.PDPXMD 位必须置1。
	//0 = MAC工作在半双工模式下。 PHCON1.PDPXMD 位必须清零。
	ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET,MACON3,MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN|MACON3_FULDPX);
	// set inter-frame gap (non-back-to-back)
	//配置非背对背包间间隔寄存器的低字节
	//MAIPGL。 大多数应用使用12h 编程该寄存器。
	//如果使用半双工模式，应编程非背对背包间间隔
	//寄存器的高字节MAIPGH。 大多数应用使用0Ch
	//编程该寄存器。
	ENC28J60_Write(MAIPGL,0x12);
	ENC28J60_Write(MAIPGH,0x0C);
	// set inter-frame gap (back-to-back)
	//配置背对背包间间隔寄存器MABBIPG。当使用
	//全双工模式时，大多数应用使用15h 编程该寄存
	//器，而使用半双工模式时则使用12h 进行编程。
	ENC28J60_Write(MABBIPG,0x15);
	// Set the maximum packet size which the controller will accept
	// Do not send packets longer than MAX_FRAMELEN:
	// 最大帧长度  1500
	ENC28J60_Write(MAMXFLL,MAX_FRAMELEN&0xFF);	
	ENC28J60_Write(MAMXFLH,MAX_FRAMELEN>>8);
	// do bank 3 stuff
	// write MAC address
	// NOTE: MAC address in ENC28J60 is byte-backward
	//设置MAC地址
	ENC28J60_Write(MAADR5,enc29j60_mac[0]);	
	ENC28J60_Write(MAADR4,enc29j60_mac[1]);
	ENC28J60_Write(MAADR3,enc29j60_mac[2]);
	ENC28J60_Write(MAADR2,enc29j60_mac[3]);
	ENC28J60_Write(MAADR1,enc29j60_mac[4]);
	ENC28J60_Write(MAADR0,enc29j60_mac[5]);
	//配置PHY为全双工  LEDB为拉电流
	ENC28J60_PHY_Write(PHCON1,PHCON1_PDPXMD);	 
	// no loopback of transmitted frames	 禁止环回
	//HDLDIS：PHY 半双工环回禁止位
	//当PHCON1.PDPXMD = 1 或PHCON1.PLOOPBK = 1 时：
	//此位可被忽略。
	//当PHCON1.PDPXMD = 0 且PHCON1.PLOOPBK = 0 时：
	//1 = 要发送的数据仅通过双绞线接口发出
	//0 = 要发送的数据会环回到MAC 并通过双绞线接口发出
	ENC28J60_PHY_Write(PHCON2,PHCON2_HDLDIS);
	// switch to bank 0
	//ECON1 寄存器
	//寄存器3-1 所示为ECON1 寄存器，它用于控制
	//ENC28J60 的主要功能。 ECON1 中包含接收使能、发
	//送请求、DMA 控制和存储区选择位。	   
	ENC28J60_Set_Bank(ECON1);
	// enable interrutps
	//EIE： 以太网中断允许寄存器
	//bit 7 INTIE： 全局INT 中断允许位
	//1 = 允许中断事件驱动INT 引脚
	//0 = 禁止所有INT 引脚的活动（引脚始终被驱动为高电平）
	//bit 6 PKTIE： 接收数据包待处理中断允许位
	//1 = 允许接收数据包待处理中断
	//0 = 禁止接收数据包待处理中断
	ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET,EIE,EIE_INTIE|EIE_PKTIE);
	// enable packet reception
	//bit 2 RXEN：接收使能位
	//1 = 通过当前过滤器的数据包将被写入接收缓冲器
	//0 = 忽略所有接收的数据包
	ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET,ECON1,ECON1_RXEN);
	/*
	// Rx start
	ENC28J60_Write(ERXSTL, RXSTART_INIT&0xFF);
	ENC28J60_Write(ERXSTH, RXSTART_INIT>>8);
	// set receive pointer address
	ENC28J60_Write(ERXRDPTL, RXSTOP_INIT&0xFF);
	ENC28J60_Write(ERXRDPTH, RXSTOP_INIT>>8);
	// RX end
	ENC28J60_Write(ERXNDL, RXSTOP_INIT&0xFF);
	ENC28J60_Write(ERXNDH, RXSTOP_INIT>>8);

	// TX start
	ENC28J60_Write(ETXSTL, TXSTART_INIT&0xFF);
	ENC28J60_Write(ETXSTH, TXSTART_INIT>>8);
	// set transmission pointer address
	ENC28J60_Write(EWRPTL, TXSTART_INIT&0xFF);
	ENC28J60_Write(EWRPTH, TXSTART_INIT>>8);
	// TX end
	ENC28J60_Write(ETXNDL, TXSTOP_INIT&0xFF);
	ENC28J60_Write(ETXNDH, TXSTOP_INIT>>8);

	// do bank 1 stuff, packet filter:
    // For broadcast packets we allow only ARP packtets
    // All other packets should be unicast only for our mac (MAADR)
    //
    // The pattern to match on is therefore
    // Type     ETH.DST
    // ARP      BROADCAST
    // 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
    // in binary these poitions are:11 0000 0011 1111
    // This is hex 303F->EPMM0=0x3f,EPMM1=0x30
	ENC28J60_Write(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_BCEN);

	// do bank 2 stuff
	// enable MAC receive
	ENC28J60_Write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
	// enable automatic padding to 60bytes and CRC operations
	// ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);
	ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN | MACON3_FULDPX);
	// bring MAC out of reset

	// set inter-frame gap (back-to-back)
	// ENC28J60_Write(MABBIPG, 0x12);
	ENC28J60_Write(MABBIPG, 0x15);

	ENC28J60_Write(MACON4, MACON4_DEFER);
	ENC28J60_Write(MACLCON2, 63);

	// set inter-frame gap (non-back-to-back)
	ENC28J60_Write(MAIPGL, 0x12);
	ENC28J60_Write(MAIPGH, 0x0C);

	// Set the maximum packet size which the controller will accept
	// Do not send packets longer than MAX_FRAMELEN:
	ENC28J60_Write(MAMXFLL, MAX_FRAMELEN&0xFF);
	ENC28J60_Write(MAMXFLH, MAX_FRAMELEN>>8);

    // do bank 3 stuff
    // write MAC address
    // NOTE: MAC address in ENC28J60 is byte-backward
    ENC28J60_Write(MAADR0, enc29j60_mac[5]);
    ENC28J60_Write(MAADR1, enc29j60_mac[4]);
    ENC28J60_Write(MAADR2, enc29j60_mac[3]);
    ENC28J60_Write(MAADR3, enc29j60_mac[2]);
    ENC28J60_Write(MAADR4, enc29j60_mac[1]);
    ENC28J60_Write(MAADR5, enc29j60_mac[0]);

	// output off 
	ENC28J60_Write(ECOCON, 0x00);

	// ENC28J60_PHY_Write(PHCON1, 0x00);
	ENC28J60_PHY_Write(PHCON1, PHCON1_PDPXMD); // full duplex
    // no loopback of transmitted frames
	ENC28J60_PHY_Write(PHCON2, PHCON2_HDLDIS);

	ENC28J60_Set_Bank(ECON2);
	ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_AUTOINC);

	// switch to bank 0
	ENC28J60_Set_Bank(ECON1);
	// enable interrutps
	ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE|EIR_TXIF);
	// enable packet reception
	ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);

	// clock out 
	// enc28j60_clkout(2);

	ENC28J60_PHY_Write(PHLCON, 0xD76);	//0x476
	*/
	
	if(ENC28J60_Read(MAADR5)== enc29j60_mac[0])return 0;//初始化成功
	else return 1; 	  

}

const u8* ENC28J60_Get_Mac(void){
	
	return enc29j60_mac;
	
}

//读取EREVID
u8 ENC28J60_Get_EREVID(void)
{
	//在EREVID 内也存储了版本信息。 EREVID 是一个只读控
	//制寄存器，包含一个5 位标识符，用来标识器件特定硅片
	//的版本号
	return ENC28J60_Read(EREVID);
}
//通过ENC28J60发送数据包到网络
//len:数据包大小
//packet:数据包
u8 ENC28J60_Packet_Send(u8* packet,u32 len)
{
	u32 level;
	level = ENC28J60_Interrupt_Disable();
	//设置发送缓冲区地址写指针入口
	ENC28J60_Write(EWRPTL,TXSTART_INIT&0xFF);
	ENC28J60_Write(EWRPTH,TXSTART_INIT>>8);
	//设置TXND指针，以对应给定的数据包大小	   
	ENC28J60_Write(ETXNDL,(TXSTART_INIT+len)&0xFF);
	ENC28J60_Write(ETXNDH,(TXSTART_INIT+len)>>8);
	//写每包控制字节（0x00表示使用macon3的设置） 
	ENC28J60_Write_Op(ENC28J60_WRITE_BUF_MEM,0,0x00);
	//复制数据包到发送缓冲区
 	ENC28J60_Write_Buf(len,packet);
 	//发送数据到网络
	ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET,ECON1,ECON1_TXRTS);
	//复位发送逻辑的问题。参见Rev. B4 Silicon Errata point 12.
	if((ENC28J60_Read(EIR)&EIR_TXERIF))ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR,ECON1,ECON1_TXRTS);
	ENC28J60_Interrupt_Enable(level);
	
	return 0;
}
//从网络获取一个数据包内容
//maxlen:数据包最大允许接收长度
//packet:数据包缓存区
//返回值:收到的数据包长度(字节)									  
u32 ENC28J60_Packet_Receive(u32 maxlen,u8* packet)
{
	u32 rxstat;
	u32 len;   
	u32 level;
	level = ENC28J60_Interrupt_Disable();
//	if(ENC28J60_Read(EPKTCNT)==0)return 0;  //是否收到数据包?	   
	//设置接收缓冲器读指针
	ENC28J60_Write(ERDPTL,(NextPacketPtr));
	ENC28J60_Write(ERDPTH,(NextPacketPtr)>>8);	   
	// 读下一个包的指针
	NextPacketPtr=ENC28J60_Read_Op(ENC28J60_READ_BUF_MEM,0);
	NextPacketPtr|=ENC28J60_Read_Op(ENC28J60_READ_BUF_MEM,0)<<8;
	//读包的长度
	len=ENC28J60_Read_Op(ENC28J60_READ_BUF_MEM,0);
	len|=ENC28J60_Read_Op(ENC28J60_READ_BUF_MEM,0)<<8;
 	len-=4; //去掉CRC计数
	//读取接收状态
	rxstat=ENC28J60_Read_Op(ENC28J60_READ_BUF_MEM,0);
	rxstat|=ENC28J60_Read_Op(ENC28J60_READ_BUF_MEM,0)<<8;
	//限制接收长度	
	if (len>maxlen-1)len=maxlen-1;	
	//检查CRC和符号错误
	// ERXFCON.CRCEN为默认设置,一般我们不需要检查.
	if((rxstat&0x80)==0)len=0;//无效
	else ENC28J60_Read_Buf(len,packet);//从接收缓冲器中复制数据包	    
	//RX读指针移动到下一个接收到的数据包的开始位置 
	//并释放我们刚才读出过的内存
	ENC28J60_Write(ERXRDPTL,(NextPacketPtr));
	ENC28J60_Write(ERXRDPTH,(NextPacketPtr)>>8);
	//递减数据包计数器标志我们已经得到了这个包 
 	ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET,ECON2,ECON2_PKTDEC);
	ENC28J60_Interrupt_Enable(level);
	return(len);
}

u32 ENC28J60_Interrupt_Disable()
{
		u32 level;

    /* switch to bank 0 */
    ENC28J60_Set_Bank(EIE);

    /* get last interrupt level */
		level = ENC28J60_Read(EIE);
    /* disable interrutps */
    ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR, EIE, level);

    return level;
}

void ENC28J60_Interrupt_Enable(u32 level)
{
    /* switch to bank 0 */
    ENC28J60_Set_Bank(EIE);
    ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET, EIE, level);
}
/*
#ifdef RT_USING_FINSH

// Debug routine to dump useful register contents

static void enc28j60(void)
{
	rt_kprintf("-- enc28j60 registers:\n");
	rt_kprintf("HwRevID: 0x%02x\n", spi_read(EREVID));
	rt_kprintf("Cntrl: ECON1 ECON2 ESTAT  EIR  EIE\n");
	rt_kprintf("       0x%02x  0x%02x  0x%02x  0x%02x  0x%02x\n",spi_read(ECON1), spi_read(ECON2), spi_read(ESTAT), spi_read(EIR), spi_read(EIE));
	rt_kprintf("MAC  : MACON1 MACON3 MACON4\n");
	rt_kprintf("       0x%02x   0x%02x   0x%02x\n", spi_read(MACON1), spi_read(MACON3), spi_read(MACON4));
	rt_kprintf("Rx   : ERXST  ERXND  ERXWRPT ERXRDPT ERXFCON EPKTCNT MAMXFL\n");
	rt_kprintf("       0x%04x 0x%04x 0x%04x  0x%04x  ",
		(spi_read(ERXSTH) << 8) | spi_read(ERXSTL),
		(spi_read(ERXNDH) << 8) | spi_read(ERXNDL),
		(spi_read(ERXWRPTH) << 8) | spi_read(ERXWRPTL),
		(spi_read(ERXRDPTH) << 8) | spi_read(ERXRDPTL));
	rt_kprintf("0x%02x    0x%02x    0x%04x\n", spi_read(ERXFCON), spi_read(EPKTCNT),
		(spi_read(MAMXFLH) << 8) | spi_read(MAMXFLL));

	rt_kprintf("Tx   : ETXST  ETXND  MACLCON1 MACLCON2 MAPHSUP\n");
	rt_kprintf("       0x%04x 0x%04x 0x%02x     0x%02x     0x%02x\n",
		(spi_read(ETXSTH) << 8) | spi_read(ETXSTL),
		(spi_read(ETXNDH) << 8) | spi_read(ETXNDL),
		spi_read(MACLCON1), spi_read(MACLCON2), spi_read(MAPHSUP));
}
#include <finsh.h>
FINSH_FUNCTION_EXPORT(enc28j60, dump enc28j60 registers);
#endif*/


// * RX handler
// * ignore PKTIF because is unreliable! (look at the errata datasheet)
// * check EPKTCNT is the suggested workaround.
// * We don't need to clear interrupt flag, automatically done when
// * enc28j60_hw_rx() decrements the packet counter.

/*
 * RX handler
 * ignore PKTIF because is unreliable! (look at the errata datasheet)
 * check EPKTCNT is the suggested workaround.
 * We don't need to clear interrupt flag, automatically done when
 * enc28j60_hw_rx() decrements the packet counter.
 */
#include "n2IP.h"
extern Netif_t eth_if;
static u8 eth_buf[1600];

void ENC28J60_queryData(void){
			
	/* Variable definitions can be made now. */
	volatile unsigned int eir, pk_counter;
	volatile unsigned char rx_activiated;

	rx_activiated = 0;

	/* get EIR */
	eir = ENC28J60_Read(EIR);
	
	//handle the INT.
	do{
			/* errata #4, PKTIF does not reliable */
	    pk_counter = ENC28J60_Read(EPKTCNT);
	    if (pk_counter){
					u32 level,eth_len;
					/* disable enc28j60 interrupt */
					level = ENC28J60_Interrupt_Disable();

					//handler Ethernet packet.
					eth_len = ENC28J60_Packet_Receive(eth_len,eth_buf);
					if(eth_len > 0){
						eth_if.low_input(&eth_if, eth_buf, eth_len);
					}

					/* enable enc28j60 interrupt */
					ENC28J60_Interrupt_Enable(level);
			
	    }

			/* clear PKTIF */
			if (eir & EIR_PKTIF){
				ENC28J60_Set_Bank(EIR);
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR, EIR, EIR_PKTIF);

				rx_activiated = 1;
			}

			/* clear DMAIF */
			if (eir & EIR_DMAIF){
				ENC28J60_Set_Bank(EIR);
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR, EIR, EIR_DMAIF);
			}

	    /* LINK changed handler */
	    if ( eir & EIR_LINKIF){
	      ENC28J60_Check_Link_Status();
	      /* read PHIR to clear the flag */
	      ENC28J60_PHY_Read(PHIR);
				ENC28J60_Set_Bank(EIR);
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR, EIR, EIR_LINKIF);
	    }

			if (eir & EIR_TXIF){
				/* A frame has been transmitted. */
				ENC28J60_Set_Bank(EIR);
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR, EIR, EIR_TXIF);
			}

			/* TX Error handler */
			if ((eir & EIR_TXERIF) != 0){
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR, EIR, EIR_TXERIF);
			}
			// RXERIF there no enough room for rx.
			if((eir & EIR_RXERIF) != 0){
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR, EIR, EIR_RXERIF);
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET, EIR, EIE_RXERIE);
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET, EIR, EIE_INTIE);
			}
			eir = ENC28J60_Read(EIR);
			
	} while ((!rx_activiated && eir));
}
void ENC28J60_ISR()
{
	
	/* Variable definitions can be made now. */
	volatile unsigned int eir, pk_counter;
	volatile unsigned char rx_activiated;

	rx_activiated = 0;

	/* get EIR */
	eir = ENC28J60_Read(EIR);
	//rt_kprintf("-->eir: 0x%08x\n", eir);
	
	//handle the INT.
	do{
			/* errata #4, PKTIF does not reliable */
	    pk_counter = ENC28J60_Read(EPKTCNT);
	    if (pk_counter){
					u32 level,eth_len;
					/* disable enc28j60 interrupt */
					level = ENC28J60_Interrupt_Disable();

					//handler Ethernet packet.
					eth_len = ENC28J60_Packet_Receive(eth_len,eth_buf);
					if(eth_len > 0){
						eth_if.low_input(&eth_if, eth_buf, eth_len);
					}

					/* enable enc28j60 interrupt */
					ENC28J60_Interrupt_Enable(level);
	    }

			/* clear PKTIF */
			if (eir & EIR_PKTIF){
				ENC28J60_Set_Bank(EIR);
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR, EIR, EIR_PKTIF);

				rx_activiated = 1;
			}

			/* clear DMAIF */
			if (eir & EIR_DMAIF){
				ENC28J60_Set_Bank(EIR);
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR, EIR, EIR_DMAIF);
			}

	    /* LINK changed handler */
	    if ( eir & EIR_LINKIF){
	      ENC28J60_Check_Link_Status();
	      /* read PHIR to clear the flag */
	      ENC28J60_PHY_Read(PHIR);
				ENC28J60_Set_Bank(EIR);
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR, EIR, EIR_LINKIF);
	    }

			if (eir & EIR_TXIF){
				/* A frame has been transmitted. */
				ENC28J60_Set_Bank(EIR);
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR, EIR, EIR_TXIF);
			}

			/* TX Error handler */
			if ((eir & EIR_TXERIF) != 0){
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR, EIR, EIR_TXERIF);
			}
			// RXERIF there no enough room for rx.
			if((eir & EIR_RXERIF) != 0){
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_CLR, EIR, EIR_RXERIF);
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET, EIR, EIE_RXERIE);
				ENC28J60_Write_Op(ENC28J60_BIT_FIELD_SET, EIR, EIE_INTIE);
			}

			eir = ENC28J60_Read(EIR);
			//rt_kprintf("-->inner eir: 0x%08x\n", eir);
			
	} while ((!rx_activiated && eir));
	
}

