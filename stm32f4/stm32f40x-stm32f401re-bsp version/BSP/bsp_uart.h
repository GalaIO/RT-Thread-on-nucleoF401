
#ifndef _HELLO_UART_H
#define _HELLO_UART_H

#include "bsp_sys.h" 
#include "bsp_fifo.h"

#include "includes.h"

#define BAUD_NOR	115200

//uartFifoµÄ¾ä±ú
extern FIFO_ptr 	uart_pFifo;

//uartMutexSemµÄ¾ä±ú
extern OS_EVENT*	uart_rx_handler;

extern void UARTsendch(u8 Data);

extern u8 UARTgetch(void);

extern u8 UARTgetch__(void);

extern char const * UARTgets(void );

extern void uart1_init(u32 BaudRate);

#define UART1_START()	(uart1_init(BAUD_NOR))

#endif
