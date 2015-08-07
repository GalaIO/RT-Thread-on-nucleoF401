
#ifndef _HELLO_INTER_TEMP_H
#define _HELLO_INTER_TEMP_H

#include "bsp_user.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
extern vu16 AD_Value[2];
#define ADC1_DR_Address    ((u32)0x4001244C)
   
extern void GPIO_Configuration(void);
extern void ADC1_Configuration(void);
extern void DMA_Configuration(void);

extern u16 GetTemp(u16 advalue);
extern u16 GetVolt(u16 advalue);

#define INTER_TEMP_START()	 {GPIO_Configuration();ADC1_Configuration();DMA_Configuration();ADC_SoftwareStartConvCmd(ADC1, ENABLE);}
#define INTER_TEMP_GET()	 (GetTemp(AD_Value[1]))

#endif
