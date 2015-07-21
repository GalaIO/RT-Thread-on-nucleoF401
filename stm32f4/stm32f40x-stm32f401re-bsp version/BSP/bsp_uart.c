
#include "bsp_uart.h"

//设备输入 特殊字符
#define InBACKSP 127           //ASCII  <--  (退格键)
#define InEOL '\r'              //ASCII <CR>  (回车键)

 /*
   定义uart的缓冲格式 在uartInit中 对其进行初始化
 */
FIFO_str  uart_fifo;
FIFO_ptr  uart_pFifo=&uart_fifo;

#define OS_UART_FIFO_SIZE	(64)
#define OS_UART_GETS_SIZE	(70)  

static char uart_fifo_buf[OS_UART_FIFO_SIZE];

static char uart_gets_buf[OS_UART_GETS_SIZE];

OS_EVENT*	uart_rx_handler;

/*						
  往uart  传送一个字节
*/
void UARTsendch(u8 Data)
{ 
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));												
	USART_SendData(USART1,Data);					
}

/*
  从uart中 获取一个字符
*/
u8 UARTgetch(void)
{ 
   	u8   err;
	OSSemPend(uart_rx_handler,0,&err);
	return FIFO_OUT(uart_pFifo);
}  
//保留 查询方式
u8 UARTgetch__(void)
{ 
   	while(!(USART_GetFlagStatus(USART1,USART_FLAG_RXNE))); //USART_GetFlagStatus：得到接收状态位
														   //USART_FLAG_RXNE:接收数据寄存器非空标志位 
														   //1：忙状态  0：空闲(没收到数据，等待。。。)
	return USART_ReceiveData(USART1);					   //接收一个字符
}

/*
  从uart中 获取一段字符串
*/
char const * UARTgets(void ){
 	int i=0;
	u8   err;
	do{
		OSSemPend(uart_rx_handler,0,&err); 
		uart_gets_buf[i]=FIFO_OUT(uart_pFifo);
		UARTsendch(uart_gets_buf[i]);	  
		if(uart_gets_buf[i]!=InBACKSP)	++i;
		else if(i>0)	--i;
	}while(uart_gets_buf[i-1]!=InEOL&&i<OS_UART_GETS_SIZE) ;
	uart_gets_buf[i-1]='\0';
	//uart_gets_buf[i]='\0';
	//UARTsendch('\n');
	return uart_gets_buf;
}


void uart1_init(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;				
	
	USART_InitTypeDef USART_InitStructure;	

	//初始化 uart fifo
	FIFO_INIT(uart_pFifo,uart_fifo_buf,OS_UART_FIFO_SIZE);

	//初始化 uart Sem 信号量 句柄  初始化 为0
	uart_rx_handler=OSSemCreate(0);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 			
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   	
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	 
  	GPIO_Init(GPIOA, &GPIO_InitStructure);				   	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  				
  	GPIO_Init(GPIOA, &GPIO_InitStructure);				   	
						    	

	USART_InitStructure.USART_BaudRate            =BaudRate ;	  			
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; 	
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;	 
	USART_InitStructure.USART_Parity              = USART_Parity_No ;	 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; 
	USART_Init(USART1, &USART_InitStructure);								
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);							
	USART_Cmd(USART1, ENABLE); 

	//配置 uart1的中断向量
	NVIC_CMD(USART1_IRQChannel,2,0);

}


void USART1_IRQHandler()
{
	OSIntEnter() ;							 
	if(!(USART_GetITStatus(USART1,USART_IT_RXNE)));  
														 
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE); 
		FIFO_IN_AUTO(uart_pFifo,USART_ReceiveData(USART1));		 
		OSSemPost(uart_rx_handler);						   			 
	}  
	
	OSIntExit() ;

}

