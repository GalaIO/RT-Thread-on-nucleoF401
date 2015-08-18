
#include "bsp_mpu6050.h"

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setClockSource(uint8_t source)
*��������:	    ����  MPU6050 ��ʱ��Դ
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
*******************************************************************************/
void MPU6050_setClockSource(uint8_t source){
    IIC1writeBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);

}

/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see getFullScaleRange()
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_RA_GYRO_CONFIG
 * @see MPU6050_GCONFIG_FS_SEL_BIT
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
void MPU6050_setFullScaleGyroRange(uint8_t range) {
    IIC1writeBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*��������:	    ����  MPU6050 ���ٶȼƵ��������
*******************************************************************************/
void MPU6050_setFullScaleAccelRange(uint8_t range) {
    IIC1writeBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setSleepEnabled(uint8_t enabled)
*��������:	    ����  MPU6050 �Ƿ����˯��ģʽ
				enabled =1   ˯��
			    enabled =0   ����
*******************************************************************************/
void MPU6050_setSleepEnabled(uint8_t enabled) {
    IIC1writeBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t MPU6050_getDeviceID(void)
*��������:	    ��ȡ  MPU6050 WHO_AM_I ��ʶ	 ������ 0x68
*******************************************************************************/
uint8_t MPU6050_getDeviceID(void) {
		uint8_t temp;
    temp = I2C1_ReadOneByte(devAddr, MPU6050_RA_WHO_AM_I);
    return temp;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t MPU6050_testConnection(void)
*��������:	    ���MPU6050 �Ƿ��Ѿ�����
*******************************************************************************/
uint8_t MPU6050_testConnection(void) {
   if(MPU6050_getDeviceID() == 0x68)  //0b01101000;
   return 1;
   	else return 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
*******************************************************************************/
void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
    IIC1writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
*******************************************************************************/
void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
    IIC1writeBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050ReadAcc(short *accData)
*��������:	    ��MPU6050�ļ��ٶ�ֵ
*******************************************************************************/
void MPU6050ReadAcc(float *accData)
{
    u8 buf[6];
		short tmp;
    IIC1readBytes(devAddr,MPU6050_ACC_OUT, 6,buf);
    tmp = (buf[0] << 8) | buf[1];
		accData[0] = 1.0*tmp / MPU6050_ACCEL_FS_2_LSB; 
    tmp = (buf[2] << 8) | buf[3];
		accData[1] = 1.0*tmp / MPU6050_ACCEL_FS_2_LSB;
    tmp = (buf[4] << 8) | buf[5];
		accData[2] = 1.0*tmp / MPU6050_ACCEL_FS_2_LSB;
}
void MPU6050ReadAccXYZ(int *ax,int *ay,int *az)
{
    u8 buf[6];
    IIC1readBytes(devAddr,MPU6050_ACC_OUT, 6,buf);
    *ax = (buf[0] << 8) | buf[1];
    *ay = (buf[2] << 8) | buf[3];
    *az = (buf[4] << 8) | buf[5];
		
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050ReadGyro(short *gyroData)
*��������:	     ��MPU6050�ĽǼ��ٶ�ֵ
*******************************************************************************/
void MPU6050ReadGyro(float *gyroData)
{
    u8 buf[6];
		short tmp;
    IIC1readBytes(devAddr,MPU6050_GYRO_OUT,6,buf);
    tmp = (buf[0] << 8) | buf[1];
		gyroData[0] = 1.0*tmp / MPU6050_GYRO_FS_1000_LSB; 
    tmp = (buf[2] << 8) | buf[3];
		gyroData[1] = 1.0*tmp / MPU6050_GYRO_FS_1000_LSB; 
    tmp = (buf[4] << 8) | buf[5];
		gyroData[2] = 1.0*tmp / MPU6050_GYRO_FS_1000_LSB; 
}
void MPU6050ReadGyroXYZ(int *gx,int *gy,int *gz)
{
    u8 buf[6];
    IIC1readBytes(devAddr,MPU6050_GYRO_OUT,6,buf);
    *gx = (buf[0] << 8) | buf[1];
    *gy = (buf[2] << 8) | buf[3];
    *gz = (buf[4] << 8) | buf[5];
}
/**************************************************************************
�������ܣ���ȡMPU6050�����¶ȴ���������
��ڲ�������
����  ֵ�������¶�
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int Read_Temperature(void)
{	   
	  float Temp;
	  Temp=(I2C1_ReadOneByte(devAddr,MPU6050_RA_TEMP_OUT_H)<<8)+I2C1_ReadOneByte(devAddr,MPU6050_RA_TEMP_OUT_L);
		if(Temp>32768) Temp-=65536;
		Temp=((float)36.53+Temp/340)*10;
	  return (int)Temp;
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void MPU6050_initialize(void)
*��������:	    ��ʼ�� 	MPU6050 �Խ������״̬��
*******************************************************************************/
void MPU6050_init(void) {
    MPU6050_setClockSource(MPU6050_CLOCK_PLL_YGYRO); //����ʱ��
    MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_1000);//������������� +-1000��ÿ��
    MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_2);	//���ٶȶ�������� +-2G
    MPU6050_setSleepEnabled(0); //���빤��״̬
}

