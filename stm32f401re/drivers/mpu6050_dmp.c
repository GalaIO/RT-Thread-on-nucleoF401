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
#if defined(RT_USING_IIC1) && defined(RT_USING_MPU6050) && defined(RT_USING_DMP)

//implement the common interface rt_err_t init(rt_device_t dev).
static rt_err_t dmp_device_init(rt_device_t dev){
	
		//only using user_data in INT_RX mode.
		if(dev->flag & RT_DEVICE_FLAG_INT_RX){
			GFifo_t *fifo = (GFifo_t *)dev->user_data;
			GFIFO_REINIT(fifo);
			//triger the interrupt.
			
		}
	
		return RT_EOK;
}

//implement the common interface rt_size_t read(rt_device_t dev,rt_off_t pos, void *buffer, rt_size_t size).
static rt_size_t dmp_device_read(rt_device_t dev,rt_off_t pos, void *buffer, rt_size_t size){

		rt_base_t	level;
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
			}else{
				RT_DEBUG_LOG(RT_DMP_DEVICE,("read DMP FIFO ERROR \r\n"));
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
			}else{
				RT_DEBUG_LOG(RT_DMP_DEVICE,("direct read DMP ERROR \r\n"));
			}
			
			/* enable interrupt, leaving the critical code area, can't be interrupted.*/
			rt_hw_interrupt_enable(level);
		}
		
		return sizeof(DMP_t);

}


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
			case DMP_CMD_GET_TEMP:
					*(int *)args = Read_Temperature();
			
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

//define the fifo room as global variable.
static DMP_t dmp_tmp[DMP_DEVICE_FIFO_SIZE];
static GFifo_t Fifo;

//implement the common interface rt_err_t rt_device_register(rt_device_t device, const char* name, rt_uint32_t flag).
rt_err_t dmp_device_register(const char* name)
{
	
	/*
	 *you should init the data of Fifo.
	**/
	
	GFIFO_INIT(&Fifo,dmp_tmp,DMP_DEVICE_FIFO_SIZE);
	
	dmp_device.type 				= RT_Device_Class_Unknown;
	dmp_device.rx_indicate 	= RT_NULL;
	dmp_device.tx_complete 	= RT_NULL;
	dmp_device.init 				= dmp_device_init;
	dmp_device.open					= RT_NULL;
	dmp_device.close				= RT_NULL;
	dmp_device.read 				= dmp_device_read;
	dmp_device.write 				= RT_NULL;
	dmp_device.control 			= dmp_device_control;
	dmp_device.user_data		= &Fifo;

	/* register a character device must input RDONLY or WRONLY or RDWR and DEACTIVATE*/
	return rt_device_register(&dmp_device, name, RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_STREAM | RT_DEVICE_FLAG_RDONLY | RT_DEVICE_FLAG_DEACTIVATE);
}

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
			}else{
				RT_DEBUG_LOG(RT_DMP_DEVICE,("in mpu6050 isr,direct read DMP ERROR \r\n"));
			}
			//force enqueue.
			GFIFO_IN_FORCE(fifo,tmp,DMP_t);
			
			/* enable interrupt */
			rt_hw_interrupt_enable(level);
			
			
			/*@added GalaIO, release the rx_sem*/
			rt_device_Rx_Done(dev);
	}
	
}

#endif /*RT_USING_DMP RT_USING_MPU6050 RT_USING_IIC1*/
