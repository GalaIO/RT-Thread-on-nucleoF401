
#include "bsp_sys.h"


//ʹ�ܸ��ж� �������Ӧ���ȼ�
void NVIC_CMD(u8 channel,u8 pre,u8 sub){

	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = (channel);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (pre);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = (sub);
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
  delay����	 �������ӳ� 
  ��Ϊc����ִ�е���Ч�����⣬���ͻ������һ�£�
  ������һ����䣬һ�����ڣ�ֻ���ڻ�������һ��ָ��һ������
  ���Ա������У׼���������У׼�õ�ʱ�䡣
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
