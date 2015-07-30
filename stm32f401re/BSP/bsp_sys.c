
/*
 *	Author:		GalaIO
 *	Date:			2015-7-26 10:12 AM
 *	Description:
 *			define the delay call.
 *			-delay count ms , and do nothing in the meanwhile.
 *				delayMS(int count)
 *			-delay count us , and do nothing in the meanwhile.
 *				delayUS(int count)
 *			-code the nvic configuration , but declare in bsp_user.h.
 *				NVIC_CMD(u8 channel,u8 pre,u8 sub)
**/
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
