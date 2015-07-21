#ifndef __SPI_ENC28J60_H
#define __SPI_ENC28J60_H

#include "bsp_sys.h"

#define 	SPI2_CS	 		GPIO_Pin_12												/* ENC28J60片选线 */
#define 	SPI2_CSL()		GPIOB->BRR = SPI2_CS;					/* 拉低片选 */
#define 	SPI2_CSH()		GPIOB->BSRR = SPI2_CS;				/* 拉高片选 */

void 			SPI2_Init(void);
unsigned char	SPI2_ReadWrite(unsigned char writedat);

#endif /* __SPI_ENC28J60_H */
