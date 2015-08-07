
/*
 *	Author:		GalaIO
 *	Date:			2015-7-26 10:12 AM
 *	Description:
 *		design a RTC device in RT-Thread.
 *			-design a RTC device as a Block Device, you can read and write it by several bytes.
 *			-design a RTC device as a RTC Device, it has its own struct, read and write using a whole struct.
 *
 *	Author:		GalaIO
 *	Date:			2015-7-30 5:14 PM
 *	Description:
 *		redesign a RTC device in RT-Thread. RTC is handled by a specific struct. no user data, reduce open and close operation.
 *		
**/

#include "rtc.h"

//implement the common interface rt_err_t init(rt_device_t dev).
static rt_err_t rdt_device_init(rt_device_t dev){
		//nothing to do.
		return RT_EOK;
}

//implement the common interface rt_err_t open(rt_device_t dev,rt_uint16_t oflag).
static rt_err_t rdt_device_open(rt_device_t dev,rt_uint16_t oflag){
		//nothing to do.
		return RT_EOK;
}

//implement the common interface rt_err_t close(rt_device_t dev).
static rt_err_t rdt_device_close(rt_device_t dev){
		//return and clr all flag in open_flag.
		return RT_EOK;
}


//implement the common interface rt_size_t read(rt_device_t dev,rt_off_t pos, void *buffer, rt_size_t size).
static rt_size_t rdt_device_read(rt_device_t dev,rt_off_t pos, void *buffer, rt_size_t size){

	  //cast to RDT_t type.
		RDT_t	*buf = (RDT_t*)buffer;
		rt_base_t	level;
		if(buf == RT_NULL){
			RT_DEBUG_LOG(RT_RTC_DEVICE,("read RTC, but null buffer!!\r\n"));
			return 0;
		}
		/* disable interrupt ,enter the critical code area, can't be interrupted.*/
		level = rt_hw_interrupt_disable();
		
		RDT_gain(buf);
		
		/* enable interrupt, leaving the critical code area, can't be interrupted.*/
		rt_hw_interrupt_enable(level);
		return sizeof(RDT_t);

}

//implement the common interface rt_size_t write(rt_device_t dev,rt_off_t pos,const void *buffer,rt_size_t size).
static rt_size_t rdt_device_write(rt_device_t dev,rt_off_t pos,const void *buffer,rt_size_t size){
		
	  //cast to RDT_t type.
		RDT_t	*buf = (RDT_t*)buffer;
		rt_base_t	level;
		uint8_t i;
		if(buf == RT_NULL){
			RT_DEBUG_LOG(RT_RTC_DEVICE,("write RTC, but null buffer!!\r\n"));
			return 0;
		}
		/* disable interrupt ,enter the critical code area, can't be interrupted.*/
		level = rt_hw_interrupt_disable();
		
		i=RDT_sync(buf);
					
		/* enable interrupt, leaving the critical code area, can't be interrupted.*/
		rt_hw_interrupt_enable(level);
		if(i == 0)
			return size;
		else
			return 0;
}

//implement the common interface rt_err_t control(rt_device_t dev,rt_uint8_t cmd,void *args).
static rt_err_t rdt_device_control(rt_device_t dev,rt_uint8_t cmd,void *args){
		RDT_t tmp;
		switch(cmd){
			case RTC_CMD_GET_SUBSEC:
				//do{
					if(rdt_device_read(dev,0,&tmp,sizeof(RDT_t)) > 0){
						*(uint16_t *)args = tmp.sub_sec;
					}else{
						return RT_ERROR;
					}
				//}while(0);
				break;
			case RTC_CMD_GET_TIME:
				//you will get hour,min,sec.
					if(rdt_device_read(dev,0,&tmp,sizeof(RDT_t)) > 0){
						uint16_t *buf = (uint16_t *)args;
						buf[0] = tmp.hour;	
						buf[1] = tmp.min;
						buf[2] = tmp.sec;
					}else{
						return RT_ERROR;
					}
				break;
			case RTC_CMD_GET_DATE:
				//you will get year,month,day.
					if(rdt_device_read(dev,0,&tmp,sizeof(RDT_t)) > 0){
						uint16_t *buf = (uint16_t *)args;
						buf[0] = tmp.year;
						buf[1] = tmp.month;
						buf[2] = tmp.day;
					}else{
						return RT_ERROR;
					}
				break;
			case RTC_CMD_SET_TIME:
					if(rdt_device_read(dev,0,&tmp,sizeof(RDT_t)) > 0){
						uint16_t *buf = (uint16_t *)args;
						tmp.hour = buf[0];	
						tmp.min = buf[1];
						tmp.sec = buf[2];
						if(rdt_device_write(dev,0,&tmp,sizeof(RDT_t)) <= 0){
							return RT_ERROR;
						}
					}else{
						return RT_ERROR;
					}
				break;
			case RTC_CMD_SET_DATE:
					if(rdt_device_read(dev,0,&tmp,sizeof(RDT_t)) > 0){
						uint16_t *buf = (uint16_t *)args;
						tmp.year = buf[0];
						tmp.month = buf[1];
						tmp.day = buf[2];
						if(rdt_device_write(dev,0,&tmp,sizeof(RDT_t)) <= 0){
							return RT_ERROR;
						}
					}else{
						return RT_ERROR;
					}
				break;
			case RTC_CMD_SET_DEFAULT:
					setDefaultRTC();
				break;
			default:
				return RT_EOK;
		}
	
		return RT_EOK;
}

//define the special device handler.
static struct rt_device rdt_device;

//implement the common interface rt_err_t rt_device_register(rt_device_t device, const char* name, rt_uint32_t flag).
rt_err_t rdt_device_register(const char* name)
{
	
	/*
	 *in RTC device,  open close callback handler, can be RT_NULL.
	**/
	
	rdt_device.type 				= RT_Device_Class_RTC;
	rdt_device.rx_indicate 	= RT_NULL;
	rdt_device.tx_complete 	= RT_NULL;
	rdt_device.init 				= rdt_device_init;
	rdt_device.open					= rdt_device_open;
	rdt_device.close				= rdt_device_close;
	rdt_device.read 				= rdt_device_read;
	rdt_device.write 				= rdt_device_write;
	rdt_device.control 			= rdt_device_control;
	rdt_device.user_data		= RT_NULL;

	/* register a character device must input RDONLY or WRONLY or RDWR and DEACTIVATE*/
	return rt_device_register(&rdt_device, name, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_DEACTIVATE);
}

