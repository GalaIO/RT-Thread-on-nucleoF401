
#include "bsp_sys.h"


//使能各中断 并添加响应优先级
void NVIC_CMD(u8 channel,u8 pre,u8 sub){

	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = (channel);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (pre);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = (sub);
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
  delay函数	 用于死延迟 
  因为c语言执行的有效性问题，不和汇编语言一致，
  并不是一条语句，一个周期，只能在汇编层面上一条指令一个周期
  所以必须进行校准，下面就是校准好的时间。
*/

void delayMS(int count){
 	unsigned int tmp;
	while(count--){
	 	for(tmp=STM32F4_SYSTEM_FREQUENCE()/6/1000;tmp;tmp--);
	}
} 
void delayUS(int count){
 	unsigned int tmp;
	while(count--){
	 	for(tmp=STM32F4_SYSTEM_FREQUENCE()/6.5/1000000;tmp;tmp--);
	}
}
