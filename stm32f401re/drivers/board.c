/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      first implementation
 */

/*
 *	Author:		GalaIO
 *	Date:			2015-7-21 8:46 PM
 *	Description:
 *		Arduino interface:
 *
 *																													D15		PB8  -10-	I2C1_SCL//,TIM4_CH3
 *																													D14		PB9	 -9-	I2C1_SDA//,TIM4_CH4
 *																						   								 AVDD  -8-
 *			-1-  NC																	 								  GND	 -7-
 *			-2-  IOREF																					D13	 PB13	 -6-  User_LED//
 *			-3-  RESET																					D12		PA6  -5-	SPI1_MISO//,TIM3_CH1,ADC1_IN6
 *			-4-  +3V3																						D11		PA7  -4-	SPI1_MOSI//,TIM1_CH1N,TIM3_CH2,ADC1_IN7
 *			-5-  +5V																						D10		PB6  -3-	SPI1_CS//,TIM4_CH1
 *			-6-  GND																						D9		PC7	 -2-	//TIM3_CH2
 *			-7-  GND																						D8		PA9	 -1-	USART1_TX//,TIM1_CH2
 *			-8-  VIN
 *																													D7		PA8  -8-	//TIM1_CH1
 *			-1-  PA0	A0	TIM2_CH1//,TM5_CH1,ADC1_IN0,WKUP			D6	 PB10	 -7-	SPI3_CS|TIM2_CH3
 *			-2-  PA1	A1	TIM2_CH2//,TIM5_CH2,ADC1_IN1					D5		PB4  -6-	SPI3_MISO|TIM3_CH1
 *			-3-	 PA4	A2	ADC1_IN4//														D4		PB5  -5-	SPI3_MOSI|TIM3_CH2
 *			-4-  PB0	A3	TIM3_CH3//,TIM1_CH2N,ADC1_IN8					D3		PB3  -4-	SPI3_SCK//,TIM2_CH2
 *			-5-  PC1	A4	ADC1_IN11//														D2	 PA10  -3-	USART1_RX//,TIM1_CH3
 *      -6-	 PC0	A5	ADC1_IN10//														D1		PA2	 -2-  USART2_TX//,TIM2_CH3,TIM5_CH3,ADC1_IN2
 *																													D0	  PA3  -1-  USART2_RX//,TIM2_CH4,TIM5_CH4,ADC1_IN3
 *		Note:需要注意的是对于SPI1，PA5-SPI1_SCK//,TIM2_CH1,ADC1_IN5，不在上面。
 *					PC13-User_button。
**/


#include "stm32f4xx.h"
#include "board.h"
#include "rtuser.h"
/**
 * @addtogroup STM32
 */

//config RTC
#include "rtc.h"
#ifdef RT_USING_NUCLEOF401_RTC
#include "rtc.h"
#endif

//config MPU6050
#ifdef RT_USING_IIC1
#include "bsp_iic1.h"
#ifdef RT_USING_MPU6050
#include "bsp_mpu6050.h"
#include "mpu6050_dmp.h"
#ifdef RT_USING_DMP
#include "inv_mpu.h"
#endif
#endif
#endif

//config NRF2401
#ifdef RT_USING_SPI1
#ifdef RT_USING_NRF2401
#include "bsp_24l01.h"
#include "nrf2401.h"
#endif
#endif

//config ENC28J60
#ifdef RT_USING_SPI3
#ifdef RT_USING_ENC28J60
#include "bsp_enc28j60.h"
#include "n2IP.h"
extern Netif_t eth_if;
#endif
#endif

void nucleo_borad_init(){
		user_log(NUCLEO_BOARD, "init the borad resource!");
#ifdef RT_USING_NUCLEOF401_RTC
		//start RTC.
		user_log(NUCLEO_BOARD, "/ startup the inter-RTC!");
		RTC_START();

		user_log(NUCLEO_BOARD, "// register the RTC device!");
		rdt_device_register("rdt");
	
#endif
	
		user_log(NUCLEO_BOARD, "/ init user LED!");
		LED_INIT();
		user_log(NUCLEO_BOARD, "/ init user BUTTON!");
		BUTTON_INIT();		

#ifdef RT_USING_NUCLEOF401_TIM2
		//init TIM2 update.
		user_log(NUCLEO_BOARD, "/ init TIM2!");
		TIM2_START_US(1000);
	
#ifdef RT_USING_NUCLEOF401_TIM2_PWM	
		//init PA0 PWM
		user_log(NUCLEO_BOARD, "// init TIM2's PA0 PWM!");
		TIM2_PWM_PA0(500);
		user_log(NUCLEO_BOARD, "// init TIM2's PA1 PWM!");
		TIM2_PWM_PA1(500);
#endif

#endif
	
#ifdef RT_USING_NUCLEOF401_ADC_TEMP
		user_log(NUCLEO_BOARD, "/ init STM32 inter-TEMP!");
		ADC_INERTEMP_INIT();
#endif

#ifdef RT_USING_IIC1
		user_log(NUCLEO_BOARD, "/ init the MPU6050 with IIC1 !");
		user_log(NUCLEO_BOARD, "// init the IIC1!");
		IIC1_Init();
#ifdef RT_USING_MPU6050
	  user_log(NUCLEO_BOARD, "// init the MPU6050!");
		MPU6050_init();
		user_log(NUCLEO_BOARD, "/// init finish,get ID %x \r\n",MPU6050_getDeviceID());
		//then init the device.you can get acce and gyro data, donnot matter if the dmp is inited.
		user_log(NUCLEO_BOARD, "/// register the DMP_DEVICE on RT_Thread Kernel!");
		dmp_device_register(DMP_DEVICE_NAME);
#ifdef RT_USING_DMP
	  user_log(NUCLEO_BOARD, "// init the DMP module MPU6050!");
		while(1){
			int error_info = mpu_dmp_init();
			if ( error_info != 0 )
			{
				user_log(NUCLEO_BOARD, "/// init DMP WAITING with error %d!", error_info );
			}
			else
			{
				user_log(NUCLEO_BOARD, "/// init DMP SUCCESS!" );
				break;
			}
			LED_OFF();
			delayMS(200);
			LED_ON();
			delayMS(200);
		}
		//init the INT_RX, PA4;
		user_log(NUCLEO_BOARD, "/// init DMP IRQ(PA4) for Receiving Data!" );
		RCC_CMD(PA_PER);
		PAIN_INT(BIT4);
		//SYSCFG_ENABLE();
		PAEXTI_INT(PIN4,EXTI_F);
		GENERAL_NVIC_CMD(EXTI4_IRQn);
		
#endif
#endif
#endif
		

#ifdef RT_USING_SPI1
#ifdef RT_USING_NRF2401
		user_log(NUCLEO_BOARD, "/ init the 2401 and check it!");
		NRF24L01_Init();
		while(NRF24L01_Check()){
			user_log(NUCLEO_BOARD, "// 2401 check error!\r\n-->check the conect or code!");
			LED_OFF();
			delayMS(200);
			LED_ON();
			delayMS(200);
		}
		user_log(NUCLEO_BOARD, "// 2401 check OK!");
		dmp_device_register("2401");
		user_log(NUCLEO_BOARD, "// register nrf2401 device on RT-Thread Kernel!");
#endif
#endif		
		

#ifdef RT_USING_SPI3
#ifdef RT_USING_ENC28J60
		user_log(NUCLEO_BOARD, "/ init the enc28j60 and check it!");
		user_log(NUCLEO_BOARD, "// init %s!",ENC28J60_Init()?"Failed":"Successful");
		user_log(NUCLEO_BOARD, "// check the version is %x!",ENC28J60_Get_EREVID());
		//init ethernetif .
		user_log(NUCLEO_BOARD, "// init the eth_if with code %d!",ethernetif_initParams(&eth_if,
															(char *)ENC28J60_Get_Mac(), "219.245.65.240", "219.245.65.254", "255.255.255.0"));
//		user_log(NUCLEO_BOARD, "// init the eth_if with code %d!",ethernetif_initParams(&eth_if,
//															(char *)ENC28J60_Get_Mac(), "192.168.1.8", "192.168.1.1", "255.255.255.0"));
		//add low_output interface.
		eth_if.low_output = (err_t (*)(U8_t *,U32_t ))ENC28J60_Packet_Send;
		//init n2IP kernel.
		user_log(NUCLEO_BOARD, "// init the n2IP kernel with code %d!",n2IP_init());
#endif
#endif
		
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
		/* Set the Vector Table base location at 0x20000000 */
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
		/* Set the Vector Table base location at 0x08000000 */
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

/*******************************************************************************
 * Function Name  : SysTick_Configuration
 * Description    : Configures the SysTick for OS tick.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void  SysTick_Configuration(void)
{
	RCC_ClocksTypeDef  rcc_clocks;
	rt_uint32_t         cnts;
	
	user_log(NUCLEO_BOARD, "/ init SYSTICK for RT-Thread Schedule Tick!");
	RCC_GetClocksFreq(&rcc_clocks);

	cnts = (rt_uint32_t)rcc_clocks.HCLK_Frequency / RT_TICK_PER_SECOND;
	user_log(NUCLEO_BOARD, "// -------*****************---------");
	user_log(NUCLEO_BOARD, "//     the system clock: %u Hz", (rt_uint32_t)rcc_clocks.SYSCLK_Frequency);
	user_log(NUCLEO_BOARD, "//     the  AHB   clock: %u Hz", (rt_uint32_t)rcc_clocks.HCLK_Frequency);
	user_log(NUCLEO_BOARD, "//     the  APB1  clock: %u Hz", (rt_uint32_t)rcc_clocks.PCLK1_Frequency);
	user_log(NUCLEO_BOARD, "//     the  APB2  clock: %u Hz", (rt_uint32_t)rcc_clocks.PCLK2_Frequency);
	user_log(NUCLEO_BOARD, "// -------*****************---------");
	//check the macro configuration correct?
	if((rt_uint32_t)rcc_clocks.SYSCLK_Frequency != SYSCLK_HZ 
			&& (rt_uint32_t)rcc_clocks.PCLK1_Frequency != PCLK1_HZ
			&& (rt_uint32_t)rcc_clocks.PCLK2_Frequency != PCLK2_HZ){
			user_log(NUCLEO_BOARD, "// That's horrible ! Your configuration is wrong ,find it and trap it !\r\n");
			while(1);	
	}
	cnts = cnts / 8;

	SysTick_Config(cnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	rt_tick_increase();

	/* leave interrupt */
	rt_interrupt_leave();
}

/**
 * This function will initial STM32 board.
 */
void rt_hw_board_init()
{
	/* NVIC Configuration */
	NVIC_Configuration();

	rt_hw_usart_init();

#ifdef RT_USING_CONSOLE
	//init the console, for debug sonmethhing.
	rt_console_set_device(CONSOLE_DEVICE);
#endif
	
	/* show version */
	rt_show_version();
	
	nucleo_borad_init();
	
	/* Configure the SysTick */
	SysTick_Configuration();
	
}

/*@}*/
