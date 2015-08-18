/*
 *	Author:		GalaIO
 *	Date:			2015-8-3 8:44 PM
 *	Description: 
 *			---add a device for RT-Thread, the motion sensor, need IIC1, and bsp_mpu6050.c .
 *			---it's a stream device, with a Fifo.
 *			---RDONLY device.
 *
**/

#include "mpu6050_dmp.h"

#include "GFifo.h"

//if using the 3 function.
#if defined(RT_USING_IIC1) && defined(RT_USING_MPU6050) 

//implement the common interface rt_err_t init(rt_device_t dev).
static rt_err_t dmp_device_init(rt_device_t dev){

#if defined(RT_USING_DMP)
		//only using user_data in INT_RX mode.
		if(dev->flag & RT_DEVICE_FLAG_INT_RX){
			GFifo_t *fifo = (GFifo_t *)dev->user_data;
			GFIFO_REINIT(fifo);
			//triger the interrupt.
			
		}
#endif	
		return RT_EOK;
}

#if defined(RT_USING_DMP)
//implement the common interface rt_size_t read(rt_device_t dev,rt_off_t pos, void *buffer, rt_size_t size).
static rt_size_t dmp_device_read(rt_device_t dev,rt_off_t pos, void *buffer, rt_size_t size){

		rt_base_t	level;
		uint8_t res;
	  //cast to DMP_t type.
		DMP_t *tmp = (DMP_t *)buffer;
		if(tmp == RT_NULL){
			RT_DEBUG_LOG(RT_DMP_DEVICE,("read DMP, but null buffer!!\r\n"));
			return 0;
		}
		
		if(dev->flag & RT_DEVICE_FLAG_INT_RX){
			/* disable interrupt ,enter the critical code area, can't be interrupted.*/
			level = rt_hw_interrupt_disable();
			//wait for the semphore (dmp device's rx_sem) posted.
			if(RT_EOK != rt_device_Rx_Wait(dev)){
				return 0;
			}
			do{
			//get MPU6050 data from INT_RX fifo.
			GFifo_t *fifo = (GFifo_t *)dev->user_data;
			if(GFIFO_OUT(fifo,*tmp,DMP_t)){
				//RT_DEBUG_LOG(RT_DMP_DEVICE,("read DMP FIFO successful,Roll: %.2f\tPitch: %.2f\tYaw: %.2f\r\n",tmp->Roll,tmp->Pitch,tmp->Yaw));
				res = sizeof(DMP_t);
			}else{
				RT_DEBUG_LOG(RT_DMP_DEVICE,("read DMP FIFO ERROR \r\n"));
				res = 0;
			}
			}while(0);
			/* enable interrupt, leaving the critical code area, can't be interrupted.*/
			rt_hw_interrupt_enable(level);
		}else if(!(dev->flag & RT_DEVICE_FLAG_DMA_RX)){
			/* disable interrupt ,enter the critical code area, can't be interrupted.*/
			level = rt_hw_interrupt_disable();
			//get MPU6050 data directly.
			if(0 == mpu_dmp_get_data(&tmp->Pitch,&tmp->Roll,&tmp->Yaw)){
				//RT_DEBUG_LOG(RT_DMP_DEVICE,("direct read DMP successful,Roll: %.2f\tPitch: %.2f\tYaw: %.2f\r\n",tmp->Roll,tmp->Pitch,tmp->Yaw));
				res = sizeof(DMP_t);
			}else{
				RT_DEBUG_LOG(RT_DMP_DEVICE,("direct read DMP ERROR \r\n"));
				res = 0;
			}
			
			/* enable interrupt, leaving the critical code area, can't be interrupted.*/
			rt_hw_interrupt_enable(level);
		}
		
		return res;

}
#endif
#include "filter.h"
#include "math.h"
//implement the common interface rt_err_t control(rt_device_t dev,rt_uint8_t cmd,void *args).
static rt_err_t dmp_device_control(rt_device_t dev,rt_uint8_t cmd,void *args){
		rt_base_t	level;
		/* disable interrupt ,enter the critical code area, can't be interrupted.*/
		level = rt_hw_interrupt_disable();
		switch(cmd){
			case DMP_CMD_GET_ACCE:
					MPU6050ReadAcc((float *)args);
					
				break;
			case DMP_CMD_GET_GYRO:
					MPU6050ReadGyro((float *)args);
			
				break;
			case DMP_CMD_GET_ACCEXYZ:
					MPU6050ReadAccXYZ((int *)args,(int *)args+1,(int *)args+2);
			
				break;
			case DMP_CMD_GET_GYROXYZ:
					MPU6050ReadGyroXYZ((int *)args,(int *)args+1,(int *)args+2);
			
				break;
			case DMP_CMD_GET_TEMP:
					*(int *)args = Read_Temperature();
			
				break;
			case DMP_CMD_GET_POSTURE_FOLFILTER:
				#define POSTURE_FOLFILTER_K		0.35
				do{
					static fol_filter_t folR={
						POSTURE_FOLFILTER_K,	//K.
						1											//last. 
					};
					static fol_filter_t folP={
						POSTURE_FOLFILTER_K,	//K.
						1											//last. 
					};
					float acce[3];
					float gyro[3];
					float *temp = (float *)args;
					MPU6050ReadAcc(acce);
					MPU6050ReadGyro(gyro);
					
					folR.last = fol_filter(folR.K,folR.last+(float)0.02*gyro[0],atan(acce[1]/acce[2])*57.32);
					folP.last = fol_filter(folP.K,folP.last+(float)0.02*gyro[1],atan(acce[0]/acce[2])*57.32);
					
					temp[0] = folR.last;
					temp[1] = -folP.last;
					temp[2] = 0;
					
				}while(0);
				break;
			default:
				//just do nothing.
				break;
		}
		/* enable interrupt, leaving the critical code area, can't be interrupted.*/
		rt_hw_interrupt_enable(level);
		return RT_EOK;
}

//define the special device handler.
static struct rt_device dmp_device;

#if defined(RT_USING_DMP)
//define the fifo room as global variable.
static DMP_t dmp_tmp[DMP_DEVICE_FIFO_SIZE];
static GFifo_t Fifo;
#endif

//implement the common interface rt_err_t rt_device_register(rt_device_t device, const char* name, rt_uint32_t flag).
rt_err_t dmp_device_register(const char* name)
{
	
#if defined(RT_USING_DMP)
	/*
	 *you should init the data of Fifo.
	**/
	GFIFO_INIT(&Fifo,dmp_tmp,DMP_DEVICE_FIFO_SIZE);
#endif
	
	dmp_device.type 				= RT_Device_Class_Unknown;
	dmp_device.rx_indicate 	= RT_NULL;
	dmp_device.tx_complete 	= RT_NULL;
	dmp_device.init 				= dmp_device_init;
	dmp_device.open					= RT_NULL;
	dmp_device.close				= RT_NULL;
#if defined(RT_USING_DMP)
	dmp_device.read 				= dmp_device_read;
#else
	dmp_device.read 				= RT_NULL;
#endif
	
	dmp_device.write 				= RT_NULL;
	dmp_device.control 			= dmp_device_control;
#if defined(RT_USING_DMP)
	dmp_device.user_data		= &Fifo;
#else
	dmp_device.user_data		= RT_NULL;
#endif

#if defined(RT_USING_DMP)
	/* register a character device must input RDONLY or WRONLY or RDWR and DEACTIVATE*/
	return rt_device_register(&dmp_device, name, RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_STREAM | RT_DEVICE_FLAG_RDONLY | RT_DEVICE_FLAG_DEACTIVATE);
#else
	/* register a character device with DEACTIVATE*/
	return rt_device_register(&dmp_device, name, RT_DEVICE_FLAG_RDONLY | RT_DEVICE_FLAG_DEACTIVATE);
#endif
}

#if defined(RT_USING_DMP)
void dmp_device_isr(){
	rt_device_t dev = &dmp_device;
	if(dev->flag & RT_DEVICE_FLAG_INT_RX){
			GFifo_t *fifo = dev->user_data;
			DMP_t tmp;
			rt_base_t level;

			/* disable interrupt */
			level = rt_hw_interrupt_disable();
		
			//get MPU6050 data directly.
			if(0 == mpu_dmp_get_data(&(&tmp)->Pitch,&(&tmp)->Roll,&(&tmp)->Yaw)){
				//RT_DEBUG_LOG(RT_DMP_DEVICE,("in mpu6050 isr,direct read DMP successful,Roll: %.2f\tPitch: %.2f\tYaw: %.2f\r\n",tmp.Roll,tmp.Pitch,tmp.Yaw));
				//force enqueue.
				GFIFO_IN_FORCE(fifo,tmp,DMP_t);
			}else{
				RT_DEBUG_LOG(RT_DMP_DEVICE,("in mpu6050 isr,direct read DMP ERROR \r\n"));
			}
			
			/* enable interrupt */
			rt_hw_interrupt_enable(level);
			
			
			/*@added GalaIO, release the rx_sem*/
			rt_device_Rx_Done(dev);
	}
	
}
#endif

#endif /*RT_USING_DMP RT_USING_MPU6050 RT_USING_IIC1*/
