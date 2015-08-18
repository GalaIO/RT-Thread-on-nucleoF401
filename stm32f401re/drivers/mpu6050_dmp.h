/*
 *	Author:		GalaIO
 *	Date:			2015-8-3 8:44 PM
 *	Description: 
 *			---add a device for RT-Thread, the motion sensor, need IIC1, and bsp_mpu6050.c .
 *			---it's a stream device, with a Fifo.
 *			---RDONLY device.
 *
**/

#ifndef _MPU6050_DMP_H_
#define _MPU6050_DMP_H_

#include "bsp_user.h"
#include "board.h"
#include "rtusing.h"

#if defined(RT_USING_IIC1) && defined(RT_USING_MPU6050) 
#include "bsp_iic1.h"
#include "bsp_mpu6050.h"
#include "inv_mpu.h"

//the device name.
#define DMP_DEVICE_NAME	"dmp"

#if defined(RT_USING_DMP)
typedef struct{
	float Roll;
	float Pitch;
	float Yaw;
}DMP_t;

//define the DMP device fifo size.
#define DMP_DEVICE_FIFO_SIZE	100

#endif

//defien the cmd for controlling the device.
#define DMP_CMD_GET_ACCE		0x01
#define DMP_CMD_GET_GYRO		0x02	
#define DMP_CMD_GET_ACCEXYZ	0x03
#define DMP_CMD_GET_GYROXYZ	0x04	
#define DMP_CMD_GET_TEMP		0x05
#define DMP_CMD_GET_POSTURE_FOLFILTER 0x06

rt_err_t dmp_device_register(const char* name);

#endif /*RT_USING_MPU6050 RT_USING_IIC1*/

#endif /*_MPU6050_DMP_H_*/
