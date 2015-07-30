
/*
 *	Author:		GalaIO
 *	Date:			2015-7-26 10:12 AM
 *	Description:
 *		design a RTC device in RT-Thread.
 *			-design a RTC device as a Block Device, you can read and write it by several bytes.
 *			-design a RTC device as a RTC Device, it has its own struct, read and write using a whole struct.
**/

#include "rtc.h"

#ifdef RTC_DEVICE_USING_BLOCK

static struct rt_device rtc_device;

//implement the common interface rt_err_t init(rt_device_t dev).
static rt_err_t rtc_device_init(rt_device_t dev){
		//如果没有激活.
		if(!(dev->flag | RT_DEVICE_FLAG_ACTIVATED)){
			dev->flag |= RT_DEVICE_FLAG_ACTIVATED;
		}
		return RT_EOK;
}

//implement the common interface rt_err_t open(rt_device_t dev,rt_uint16_t oflag).
static rt_err_t rtc_device_open(rt_device_t dev,rt_uint16_t oflag){
		//if oflag equals RT_DEVICE_OFLAG_RDONLY or RT_DEVICE_OFLAG_WRONLY or RT_DEVICE_OFLAG_RDWR,
		//and not opened. then the device will be closed.
		if((RT_DEVICE_OFLAG_RDONLY == oflag || 
				RT_DEVICE_OFLAG_WRONLY == oflag || 
				RT_DEVICE_OFLAG_RDWR   == oflag) &&  (dev->flag & oflag) &&
				!(RT_DEVICE_OFLAG_OPEN & dev->open_flag)){
			dev->open_flag |= oflag;
			return RT_EOK;
		}else{
			//clr all flag in open_flag.
			//open device erro, just close device .
			dev->open_flag = RT_DEVICE_OFLAG_CLOSE;
			RT_DEBUG_LOG(RT_RTC_DEVICE,("open failed! check you oflag input or check if the device is opened!! %d %d\r\n",dev->open_flag,oflag));
			return RT_ERROR;
		}
}

//implement the common interface rt_err_t close(rt_device_t dev).
static rt_err_t rtc_device_close(rt_device_t dev){
		//return and clr all flag in open_flag.
		return RT_EOK;
}

#define RTC_DATA_MAX_LENGTH		(sizeof(rtc_timer)/sizeof(uint8_t))

//implement the common interface rt_size_t read(rt_device_t dev,rt_off_t pos, void *buffer, rt_size_t size).
static rt_size_t rtc_device_read(rt_device_t dev,rt_off_t pos, void *buffer, rt_size_t size){
		//get specific user data handle.
		//Date_t *dt = (Date_t*)dev->user_data;
	  //cast to rtc_t type.
		rtc_t	*buf = (rtc_t*)buffer;
		//judge the param pos.
		if((pos < 0) || (pos > RTC_DATA_MAX_LENGTH-1)){
			RT_DEBUG_LOG(RT_RTC_DEVICE,("wrong pos param!! %d %d\r\n",pos,RTC_DATA_MAX_LENGTH));
			return 0;
		}
		//judge the rtc_timer bounds.
		if((RTC_DATA_MAX_LENGTH < (pos+size))){
			size = RTC_DATA_MAX_LENGTH - pos ;
		}
		//the device is opened and readable.
		if((dev->open_flag & RT_DEVICE_OFLAG_OPEN) &&
				((dev->open_flag & RT_DEVICE_OFLAG_RDWR) ||
				(dev->open_flag & RT_DEVICE_OFLAG_RDONLY))){
			rt_base_t	level;
			uint8_t i;
			uint8_t *dt = (uint8_t*)&rtc_timer;
			/* disable interrupt ,enter the critical code area, can't be interrupted.*/
			level = rt_hw_interrupt_disable();
		
			Time_Get();
			//RT_DEBUG_LOG(RT_RTC_DEVICE,("hh1 %d %d\r\n",dt[0],dt[1]));
			for(i=0;size;i++,size--,pos++){
				buf[i] = dt[pos];
				if(pos == 3){
					buf[i]+=Date_Benchmark[0];
				}
				
				//RT_DEBUG_LOG(RT_RTC_DEVICE,("hh%d %d %d\r\n",i,dt[pos],buf[i]));
			}
		
			/* enable interrupt, leaving the critical code area, can't be interrupted.*/
			rt_hw_interrupt_enable(level);
			return size;
		}else{
			RT_DEBUG_LOG(RT_RTC_DEVICE,("the device is not opened correctly check oflag or open it again!!\r\n"));
			return 0;
		}
}

//implement the common interface rt_size_t write(rt_device_t dev,rt_off_t pos,const void *buffer,rt_size_t size).
static rt_size_t rtc_device_write(rt_device_t dev,rt_off_t pos,const void *buffer,rt_size_t size){
		//get specific user data handle.
		//Date_t *dt = (Date_t*)dev->user_data;
	  //cast to rtc_t type.
		rtc_t	*buf = (rtc_t*)buffer;
		//judge the param pos.
		if((pos < 0) || (pos > RTC_DATA_MAX_LENGTH)){
			RT_DEBUG_LOG(RT_RTC_DEVICE,("wrong pos param!! %d %d\r\n",pos,RTC_DATA_MAX_LENGTH));
			return 0;
		}
		//judge the rtc_timer bounds.
		if((RTC_DATA_MAX_LENGTH < (pos+size))){
			size = RTC_DATA_MAX_LENGTH - pos ;
		}
		//the device is opened and readable.
		if((dev->open_flag & RT_DEVICE_OFLAG_OPEN) &&
				((dev->open_flag & RT_DEVICE_OFLAG_RDWR) ||
				(dev->open_flag & RT_DEVICE_OFLAG_WRONLY))){
			rt_base_t	level;
			uint8_t i;
			uint8_t *dt = (uint8_t*)&rtc_timer;
			/* disable interrupt ,enter the critical code area, can't be interrupted.*/
			level = rt_hw_interrupt_disable();
		
			for(i=0;size;i++,size--,pos++){
				if(pos == 3){
					buf[i]-=Date_Benchmark[0];
				}
				dt[pos] = buf[i];
			}
			i=Time_Update((uint16_t)rtc_timer.year+Date_Benchmark[0],rtc_timer.month,rtc_timer.day,rtc_timer.hour,rtc_timer.min,rtc_timer.sec);
			
			/* enable interrupt, leaving the critical code area, can't be interrupted.*/
			rt_hw_interrupt_enable(level);
			if(i == 0)
				return size;
			else
				return 0;
		}else{
			RT_DEBUG_LOG(RT_RTC_DEVICE,("the device is not opened correctly check oflag or open it again!!\r\n"));
			return 0;
		}
}

//implement the common interface rt_err_t control(rt_device_t dev,rt_uint8_t cmd,void *args).
static rt_err_t rtc_device_control(rt_device_t dev,rt_uint8_t cmd,void *args){
		//just do nothing.
		return RT_EOK;
}

//implement the common interface rt_err_t rt_device_register(rt_device_t device, const char* name, rt_uint32_t flag).
rt_err_t rtc_device_register(const char* name)
{

	rtc_device.type 				= RT_Device_Class_Block;
	rtc_device.rx_indicate 	= RT_NULL;
	rtc_device.tx_complete 	= RT_NULL;
	rtc_device.init 				= rtc_device_init;
	rtc_device.open					= rtc_device_open;
	rtc_device.close				= rtc_device_close;
	rtc_device.read 				= rtc_device_read;
	rtc_device.write 				= rtc_device_write;
	rtc_device.control 			= rtc_device_control;
	rtc_device.user_data		= &rtc_timer;

	/* register a character device */
	return rt_device_register(&rtc_device, name, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_DEACTIVATE);
}

#endif


#ifdef RTC_DEVICE_USING_RTC_STRUCT


static struct rt_device rdt_device;

//implement the common interface rt_err_t init(rt_device_t dev).
static rt_err_t rdt_device_init(rt_device_t dev){
		//如果没有激活.
		if(!(dev->flag | RT_DEVICE_FLAG_ACTIVATED)){
			dev->flag |= RT_DEVICE_FLAG_ACTIVATED;
		}
		return RT_EOK;
}

//implement the common interface rt_err_t open(rt_device_t dev,rt_uint16_t oflag).
static rt_err_t rdt_device_open(rt_device_t dev,rt_uint16_t oflag){
		//if oflag equals RT_DEVICE_OFLAG_RDONLY or RT_DEVICE_OFLAG_WRONLY or RT_DEVICE_OFLAG_RDWR,
		//and not opened. then the device will be closed.
		if((RT_DEVICE_OFLAG_RDONLY == oflag || 
				RT_DEVICE_OFLAG_WRONLY == oflag || 
				RT_DEVICE_OFLAG_RDWR   == oflag) &&  (dev->flag & oflag) &&
				!(RT_DEVICE_OFLAG_OPEN & dev->open_flag)){
			dev->open_flag |= oflag;
			return RT_EOK;
		}else{
			//clr all flag in open_flag.
			//open device erro, just close device .
			dev->open_flag = RT_DEVICE_OFLAG_CLOSE;
			RT_DEBUG_LOG(RT_RTC_DEVICE,("open failed! check you oflag input or check if the device is opened!! %d %d\r\n",dev->open_flag,oflag));
			return RT_ERROR;
		}
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
		if(buf == RT_NULL){
			RT_DEBUG_LOG(RT_RTC_DEVICE,("read RTC, but null buffer!!\r\n"));
			return 0;
		}
		//the device is opened and readable.
		if((dev->open_flag & RT_DEVICE_OFLAG_OPEN) &&
				((dev->open_flag & RT_DEVICE_OFLAG_RDWR) ||
				(dev->open_flag & RT_DEVICE_OFLAG_RDONLY))){
			rt_base_t	level;
			/* disable interrupt ,enter the critical code area, can't be interrupted.*/
			level = rt_hw_interrupt_disable();
		
			RDT_gain(buf);
		
			/* enable interrupt, leaving the critical code area, can't be interrupted.*/
			rt_hw_interrupt_enable(level);
			return sizeof(RDT_t);
		}else{
			RT_DEBUG_LOG(RT_RTC_DEVICE,("the device is not opened correctly check oflag or open it again!!\r\n"));
			return 0;
		}
}

//implement the common interface rt_size_t write(rt_device_t dev,rt_off_t pos,const void *buffer,rt_size_t size).
static rt_size_t rdt_device_write(rt_device_t dev,rt_off_t pos,const void *buffer,rt_size_t size){
		
	  //cast to RDT_t type.
		RDT_t	*buf = (RDT_t*)buffer;
		if(buf == RT_NULL){
			RT_DEBUG_LOG(RT_RTC_DEVICE,("write RTC, but null buffer!!\r\n"));
			return 0;
		}
		//the device is opened and writeable. 
		if((dev->open_flag & RT_DEVICE_OFLAG_OPEN) &&
				((dev->open_flag & RT_DEVICE_OFLAG_RDWR) ||
				(dev->open_flag & RT_DEVICE_OFLAG_WRONLY))){
			rt_base_t	level;
			uint8_t i;
			/* disable interrupt ,enter the critical code area, can't be interrupted.*/
			level = rt_hw_interrupt_disable();
		
			i=RDT_sync(buf);
					
			/* enable interrupt, leaving the critical code area, can't be interrupted.*/
			rt_hw_interrupt_enable(level);
			if(i == 0)
				return size;
			else
				return 0;
		}else{
			RT_DEBUG_LOG(RT_RTC_DEVICE,("the device is not opened correctly check oflag or open it again!!\r\n"));
			return 0;
		}
}

//implement the common interface rt_err_t control(rt_device_t dev,rt_uint8_t cmd,void *args).
static rt_err_t rdt_device_control(rt_device_t dev,rt_uint8_t cmd,void *args){
		//just do nothing.
		return RT_EOK;
}

//implement the common interface rt_err_t rt_device_register(rt_device_t device, const char* name, rt_uint32_t flag).
rt_err_t rdt_device_register(const char* name)
{

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


#endif /*RTC_DEVICE_USING_RTC_STRUCT*/
