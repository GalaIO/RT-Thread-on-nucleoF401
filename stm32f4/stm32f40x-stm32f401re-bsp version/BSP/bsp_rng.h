#ifndef _BSP_RNG_H
#define _BSP_RNG_H	 


#include "bsp_user.h"

uint8_t RNG_Init(void);			//RNG��ʼ�� 
u32 RNG_Get_RandomNum(void);//�õ������
int RNG_Get_RandomRange(int min,int max);//����[min,max]��Χ�������
#endif

















