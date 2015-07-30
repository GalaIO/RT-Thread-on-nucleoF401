
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
    
    //������һ��ADת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
    //��Ϊ�Ѿ����ú���DMA��������AD�Զ�����ת��������Զ�������AD_Value��   
    
    while (1)
    {
        Delay();
        Temp = GetTemp(AD_Value[1]);
        Volt = GetVolt(AD_Value[0]);

        printf("��ѹ��%d.%d\t�¶ȣ�%d��\r\n", \
            Volt/100, Volt%100, Temp);
        
    }
}*/

                          

/*******************************************************************************
* Function Name  : GetTemp
* Description    : ����ADC��������¶�
* Input          : u16 advalue
* Output         : 
* Return         : u16 temp
*******************************************************************************/
u16 GetTemp(u16 advalue)
{
    vu16 Current_Temp;
    
//    ADCת�������Ժ󣬶�ȡADC_DR�Ĵ����еĽ����ת���¶�ֵ���㹫ʽ���£�
//          V25 - VSENSE
//  T(��) = ------------  + 25
//           Avg_Slope
//   V25��  �¶ȴ�������25��ʱ �������ѹ������ֵ1.43 V��
//  VSENSE���¶ȴ������ĵ�ǰ�����ѹ����ADC_DR �Ĵ����еĽ��ADC_ConvertedValue֮���ת����ϵΪ�� 
//            ADC_ConvertedValue * Vdd
//  VSENSE = --------------------------
//            Vdd_convert_value(0xFFF)
//  Avg_Slope���¶ȴ����������ѹ���¶ȵĹ�������������ֵ4.3 mV/�档

    Current_Temp = (1.42 - advalue*3.3/4096)*1000/4.35 + 25; 
    return (s16)Current_Temp;
}  


/*******************************************************************************
* Function Name  : GetVolt
* Description    : ����ADC��������ѹ
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
* Description    : GPIO����
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
* Description    : ADC1���ã�����ADCģ�����ú���У׼��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC1_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //����ת������
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 2;     //����ת�����г���Ϊ2
    ADC_Init(ADC1, &ADC_InitStructure);
    
    //ADC�����¶ȴ�����ʹ�ܣ�Ҫʹ��Ƭ���¶ȴ��������м�Ҫ��������
    ADC_TempSensorVrefintCmd(ENABLE);
    
    //����ת������1��ͨ��10
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_13Cycles5);
    //����ת������2��ͨ��16���ڲ��¶ȴ�������������ʱ��>2.2us,(239cycles)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 2, ADC_SampleTime_239Cycles5);
    
    // Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
    // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
    ADC_DMACmd(ADC1, ENABLE);
    
    // ������ADC�Զ�У׼����������ִ��һ�Σ���֤����
    // Enable ADC1 reset calibaration register 
    ADC_ResetCalibration(ADC1);
    // Check the end of ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1));

    // Start ADC1 calibaration
    ADC_StartCalibration(ADC1);
    // Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));
    // ADC�Զ�У׼����---------------
    
}

/*******************************************************************************
* Function Name  : DMA_Configuration
* Description    : DMA���ã���ADCģ���Զ���ת��������ڴ�
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
    //BufferSize=2����ΪADCת��������2��ͨ��
    //������ã�ʹ����1�������AD_Value[0]������2�������AD_Value[1]
    DMA_InitStructure.DMA_BufferSize = 2;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //ѭ��ģʽ������Bufferд�����Զ��ص���ʼ��ַ��ʼ����
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    //������ɺ�����DMAͨ��
    DMA_Cmd(DMA1_Channel1, ENABLE);
} 

