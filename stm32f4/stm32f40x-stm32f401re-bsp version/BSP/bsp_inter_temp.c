
#include "bsp_inter_temp.h"

vu16 AD_Value[2];

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*int main(void)
{
    RCC_Configuration();
    GPIO_Configuration();
    USART1_Configuration();
    DMA_Configuration();
    ADC1_Configuration();
    
    //启动第一次AD转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
    //因为已经配置好了DMA，接下来AD自动连续转换，结果自动保存在AD_Value处   
    
    while (1)
    {
        Delay();
        Temp = GetTemp(AD_Value[1]);
        Volt = GetVolt(AD_Value[0]);

        printf("电压：%d.%d\t温度：%d℃\r\n", \
            Volt/100, Volt%100, Temp);
        
    }
}*/

                          

/*******************************************************************************
* Function Name  : GetTemp
* Description    : 根据ADC结果计算温度
* Input          : u16 advalue
* Output         : 
* Return         : u16 temp
*******************************************************************************/
u16 GetTemp(u16 advalue)
{
    vu16 Current_Temp;
    
//    ADC转换结束以后，读取ADC_DR寄存器中的结果，转换温度值计算公式如下：
//          V25 - VSENSE
//  T(℃) = ------------  + 25
//           Avg_Slope
//   V25：  温度传感器在25℃时 的输出电压，典型值1.43 V。
//  VSENSE：温度传感器的当前输出电压，与ADC_DR 寄存器中的结果ADC_ConvertedValue之间的转换关系为： 
//            ADC_ConvertedValue * Vdd
//  VSENSE = --------------------------
//            Vdd_convert_value(0xFFF)
//  Avg_Slope：温度传感器输出电压和温度的关联参数，典型值4.3 mV/℃。

    Current_Temp = (1.42 - advalue*3.3/4096)*1000/4.35 + 25; 
    return (s16)Current_Temp;
}  


/*******************************************************************************
* Function Name  : GetVolt
* Description    : 根据ADC结果计算电压
* Input          : u16 advalue
* Output         : 
* Return         : u16 temp
*******************************************************************************/
u16 GetVolt(u16 advalue)
{
    return (u16)(advalue * 330 / 4096);
} 
                          
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : GPIO设置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //USART1_TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //USART1_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //ADC_CH10--> PC0
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 

}
/*******************************************************************************
* Function Name  : ADC1_Configuration
* Description    : ADC1设置（包括ADC模块配置和自校准）
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC1_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //连续转换开启
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 2;     //设置转换序列长度为2
    ADC_Init(ADC1, &ADC_InitStructure);
    
    //ADC内置温度传感器使能（要使用片内温度传感器，切记要开启它）
    ADC_TempSensorVrefintCmd(ENABLE);
    
    //常规转换序列1：通道10
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_13Cycles5);
    //常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 2, ADC_SampleTime_239Cycles5);
    
    // Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
    // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
    ADC_DMACmd(ADC1, ENABLE);
    
    // 下面是ADC自动校准，开机后需执行一次，保证精度
    // Enable ADC1 reset calibaration register 
    ADC_ResetCalibration(ADC1);
    // Check the end of ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1));

    // Start ADC1 calibaration
    ADC_StartCalibration(ADC1);
    // Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));
    // ADC自动校准结束---------------
    
}

/*******************************************************************************
* Function Name  : DMA_Configuration
* Description    : DMA设置：从ADC模块自动读转换结果至内存
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA_Configuration(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //BufferSize=2，因为ADC转换序列有2个通道
    //如此设置，使序列1结果放在AD_Value[0]，序列2结果放在AD_Value[1]
    DMA_InitStructure.DMA_BufferSize = 2;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //循环模式开启，Buffer写满后，自动回到初始地址开始传输
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    //配置完成后，启动DMA通道
    DMA_Cmd(DMA1_Channel1, ENABLE);
} 

