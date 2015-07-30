#ifndef _BSP_RNG_H
#define _BSP_RNG_H	 


#include "bsp_user.h"

uint8_t RNG_Init(void);			//RNG初始化 
u32 RNG_Get_RandomNum(void);//得到随机数
int RNG_Get_RandomRange(int min,int max);//生成[min,max]范围的随机数
#endif

















