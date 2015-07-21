/**
  ******************************************************************************
  * @file    system_stm32f4xx.h
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   CMSIS Cortex-M4 Device System Source File for STM32F4xx devices.       
  ******************************************************************************  
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************  
  */ 

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f4xx_system
  * @{
  */  
  
/**
  * @brief Define to prevent recursive inclusion
  */
#ifndef __SYSTEM_STM32F4XX_H
#define __SYSTEM_STM32F4XX_H

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup STM32F4xx_System_Includes
  * @{
  */

/**
  * @}
  */


/** @addtogroup STM32F4xx_System_Exported_types
  * @{
  */

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */
#if defined(STM32F40_41xxx)	/* STM32F40_41xxx */
#define STM32F4_SYSTEM_FREQUENCE() 	 168000000

#elif defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
#define STM32F4_SYSTEM_FREQUENCE() 	 180000000
	
#elif defined(STM32F401xx)
#define STM32F4_SYSTEM_FREQUENCE() 	 84000000

#elif defined(STM32F411xE)
#define STM32F4_SYSTEM_FREQUENCE() 	 100000000

#else
#define STM32F4_SYSTEM_FREQUENCE() 	 8000000
	111111;
#endif /* STM32F401xx */
/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Exported_Functions
  * @{
  */
  
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_STM32F4XX_H */

/**
  * @}
  */
  
/**
  * @}
  */  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
