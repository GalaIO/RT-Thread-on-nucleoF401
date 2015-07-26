
/*
 *	Author:		GalaIO
 *	Date:			2015-7-26 10:12 AM
 *	Description:
 *			Re package Flash configuration.
 *			-Define quick macro for calculate the alien 4 byte address.
 *				ADDR_ALIGN_4BYTE(addr)
 *				ADDR_ALIGN_4BYTE_NEXT(addr)
 *			-read a word for the specific addr in Flash, and define a quick macro.
 *				BSPFLASH_ReadWord(uint32_t faddr)
 *				BSPFLASH_READWORD(faddr)
 *			-read several words for the specific addr in Flash, and define a quick macro.
 *				BSPFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)
 *        BSPFLASH_READWORDS(WriteAddr,pBuffer,NumToWrite)
 *			-write several words for the specific addr in Flash, and define a quick macro.
 *				BSPFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)
 *				BSPFLASH_WRITEWORDS(WriteAddr,pBuffer,NumToWrite)
 *		
**/
#ifndef _BSP_FLASH_H_
#define _BSP_FLASH_H_

#include "bsp_user.h"


//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
 

//FLASH 扇区的起始地址
#ifdef STM32F401xE
//STM32F401RE contain 512 Flash ,96K SRAM.
//so it has 8 sectors.
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) 	//扇区6起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) 	//扇区7起始地址, 128 Kbytes 
//Stm32f401re read write from ADDR_FLASH_SECTOR_7
#define FLASH_STORAGE_START()		((uint32_t*)ADDR_FLASH_SECTOR_7)
#endif

#define ADDR_ALIGN_4BYTE(addr)			((uint32_t)addr&(uint32_t)0x00000003?(((addr>>2)+1)<<2):addr)
#define ADDR_ALIGN_4BYTE_NEXT(addr)	ADDR_ALIGN_4BYTE(ADDR_ALIGN_4BYTE(addr)+1)

uint32_t BSPFLASH_ReadWord(uint32_t faddr);		  	//读出字
#define BSPFLASH_READWORD(faddr)		BSPFLASH_ReadWord((uint32_t)faddr)
uint8_t BSPFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite);			//从指定地址开始写入指定长度的数据
#define BSPFLASH_WRITEWORDS(WriteAddr,pBuffer,NumToWrite)		BSPFLASH_Write((uint32_t)WriteAddr,(uint32_t*)pBuffer,(uint32_t)NumToWrite)
void BSPFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead);   		//从指定地址开始读出指定长度的数据
#define BSPFLASH_READWORDS(WriteAddr,pBuffer,NumToWrite)		BSPFLASH_Read((uint32_t)WriteAddr,(uint32_t*)pBuffer,(uint32_t)NumToWrite)
						   
#endif

















