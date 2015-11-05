/*
 *	Author:		GalaIO
 *	Date:			2015-8-24 5:55 PM
 *	Description: 
 *			---add a device for RT-Thread, the nrf2401 device, need SPI1, with polling check .
 *			---it's a stream device, but in the certain length.
 *			---READWRITE device.
 *			---add the control cmd of flush, that flush the info.
**/

#include "nrf2401.h"
#include "bsp_24l01.h"

#ifdef RT_USING_NRF2401

//nrf2401 mode macro.
#define NRF2401_MODE_NONE	0x00
#define NRF2401_MODE_RX		0x01
#define NRF2401_MODE_TX		0x02


#define NRF2401_USERDATA_MAXDATA	31
static struct nrf2401_userData{
	//none or rx or tx
	char mode;
	//must >=0 and <=31.
	char len;
	//the real data.
	char data[31];
}userData;

//implement the common interface rt_err_t init(rt_device_t dev).
static rt_err_t nrf2401_device_init(rt_device_t dev){
		
		//init the User Data.
		struct nrf2401_userData *data = (struct nrf2401_userData *)dev->user_data;
		data->mode = NRF2401_MODE_NONE;
		data->len = 0;
		RT_DEBUG_LOG(RT_NRF2401_DEVICE,("--->init the nrf2401 device!\r\n"));
		return RT_EOK;
}


//implement the common interface rt_size_t read(rt_device_t dev,rt_off_t pos, void *buffer, rt_size_t size).
static rt_size_t nrf2401_device_read(rt_device_t dev,rt_off_t pos, void *buffer, rt_size_t size){

		char i;
		char *buf = (char *)buffer;
		//get Dev user data.
		struct nrf2401_userData *data = (struct nrf2401_userData *)dev->user_data;
		//if the buf has data
		if(data->len > 0){
			if(size > data->len){
				//copy to buffer
				for(i=0;i<data->len;i++,buf++){
					*buf = data->data[i];
				}
				size -= data->len;
			}else{
				//copy to buffer
				for(i=0;i<size;i++,buf++){
					*buf = data->data[i];
				}
				//alloc the rest data
				for(i=size;i<data->len;i++){
					data->data[i-size] = data->data[i];
				}
				data->len -= size;
				return size;
			}
		}
		//Polling read mode.
		if(data->mode != NRF2401_MODE_RX){
			//if not in rx-mode, set it!
			NRF24L01_RX_Mode();
			data->mode = NRF2401_MODE_RX;
		}
		if(dev->flag & RT_DEVICE_FLAG_INT_RX){
			//INT_RX mode.
			
		}else if(!(dev->flag & RT_DEVICE_FLAG_DMA_RX)){
			//polling read 
			while(size){
				while(NRF24L01_RxPacket((u8 *)&(data->len))){
					rt_thread_delay(5);
					RT_DEBUG_LOG(RT_NRF2401_DEVICE,("--->wait for rx in 2401!\r\n"));
				}
				if(size > data->len){
					size -= data->len;
					//copy to buffer
					for(i=0;i<data->len;i++,buf++){
						*buf = data->data[i];
					}
					//correction the data->len
					data->len = 0;
				}else{
					//copy to buffer
					for(i=0;i<size;i++,buf++){
						*buf = data->data[i];
					}
					//alloc the rest data
					for(i=size;i<data->len;i++){
						data->data[i-size] = data->data[i];
					}
					data->len -= size;
					size = 0;
				}
			
			}
			return 0;
			
		}
		
		return 0;
}
/*
//flush buf-data autolly.
//implement the common interface rt_size_t write(rt_device_t dev,rt_off_t pos,const void *buffer,rt_size_t size).
static rt_size_t nrf2401_device_write(rt_device_t dev,rt_off_t pos,const void *buffer,rt_size_t size){
		
		char i;
		char *buf = (char *)buffer;
		//get Dev user data.
		struct nrf2401_userData *data = (struct nrf2401_userData *)dev->user_data;
		
		//Polling write mode.
		if(data->mode != NRF2401_MODE_TX){
			//if not in tx-mode, set it!
			NRF24L01_TX_Mode();
			//delay wait for tx-mode success.
			rt_thread_delay(1);
			data->mode = NRF2401_MODE_TX;
		}
		do{
				data->len = 0;
				if(size <= NRF2401_USERDATA_MAXDATA){
					for(i=0;i<size;i++,buf++){
						data->data[i] = *buf;
					}
					data->len = size;
					size = 0;
				}else{
					for(i=0;i<NRF2401_USERDATA_MAXDATA;i++,buf++){
						data->data[i] = *buf;
					}
					size -= NRF2401_USERDATA_MAXDATA;
					data->len = NRF2401_USERDATA_MAXDATA;
				}
				//flush the data.
				while(TX_OK != NRF24L01_TxPacket((u8 *)&(data->len))){
					RT_DEBUG_LOG(RT_NRF2401_DEVICE,("--->tx error in 2401!\r\n"));
				}
		}while(size);
		data->len = 0;
		return 0;
}


//implement the common interface rt_err_t control(rt_device_t dev,rt_uint8_t cmd,void *args).
static rt_err_t nrf2401_device_control(rt_device_t dev,rt_uint8_t cmd,void *args){
		switch(cmd){
			case 1:
				break;
			default:
				//just do nothing.
				break;
		}
		return RT_EOK;
}*/

//write bsp handler donot flush buf-data.
//implement the common interface rt_size_t write(rt_device_t dev,rt_off_t pos,const void *buffer,rt_size_t size).
static rt_size_t nrf2401_device_write(rt_device_t dev,rt_off_t pos,const void *buffer,rt_size_t size){
		
		char i;
		char *buf = (char *)buffer;
		//get Dev user data.
		struct nrf2401_userData *data = (struct nrf2401_userData *)dev->user_data;
	
		while(size){
				if(size+data->len < NRF2401_USERDATA_MAXDATA){
					for(i=0;i<size;i++,buf++){
						data->data[i+data->len] = *buf;
					}
					data->len += size;
					return 0;
				}else{
					char tmp = NRF2401_USERDATA_MAXDATA-data->len;
					for(i=0;i<tmp;i++,buf++){
						data->data[i+data->len] = *buf;
					}
					data->len = NRF2401_USERDATA_MAXDATA;
					size -= tmp;
					//now the buf is full, flush it.
					//Polling write mode.
					if(data->mode != NRF2401_MODE_TX){
						//if not in tx-mode, set it!
						NRF24L01_TX_Mode();
						//delay wait for tx-mode success.
						rt_thread_delay(1);
						data->mode = NRF2401_MODE_TX;
					}
					while(TX_OK != NRF24L01_TxPacket((u8 *)&(data->len))){
						RT_DEBUG_LOG(RT_NRF2401_DEVICE,("--->tx error in 2401!\r\n"));
					}
					data->len = 0;
				}
		}
		
		return 0;
}


//implement the common interface rt_err_t control(rt_device_t dev,rt_uint8_t cmd,void *args).
static rt_err_t nrf2401_device_control(rt_device_t dev,rt_uint8_t cmd,void *args){
		switch(cmd){
			case NRF2401_DEVICE_FLUSH:
				//get Dev user data.
				do{
					struct nrf2401_userData *data = (struct nrf2401_userData *)dev->user_data;
					if(data->len > 0){
						//now the buf is full, flush it.
						//Polling write mode.
						if(data->mode != NRF2401_MODE_TX){
							//if not in tx-mode, set it!
							NRF24L01_TX_Mode();
							//delay wait for tx-mode success.
							rt_thread_delay(1);
							data->mode = NRF2401_MODE_TX;
						}
						while(TX_OK != NRF24L01_TxPacket((u8 *)&(data->len))){
							RT_DEBUG_LOG(RT_NRF2401_DEVICE,("--->tx error in 2401!\r\n"));
						}
						data->len = 0;
					}
				}while(0);	
				break;
			default:
				//just do nothing.
				break;
		}
		return RT_EOK;
}

//define the special device handler.
static struct rt_device nrf2401_device;

//implement the common interface rt_err_t rt_device_register(rt_device_t device, const char* name, rt_uint32_t flag).
rt_err_t dmp_device_register(const char* name)
{
	
	nrf2401_device.type 				= RT_Device_Class_Block;
	nrf2401_device.rx_indicate 			= RT_NULL;
	nrf2401_device.tx_complete 			= RT_NULL;
	nrf2401_device.init 				= nrf2401_device_init;
	nrf2401_device.open					= RT_NULL;
	nrf2401_device.close				= RT_NULL;
	nrf2401_device.read 				= nrf2401_device_read;
	nrf2401_device.write 				= nrf2401_device_write;
	nrf2401_device.control 				= nrf2401_device_control;
	nrf2401_device.user_data			= (void *)&userData;

	/* register a character device with DEACTIVATE*/
	return rt_device_register(&nrf2401_device, name, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_DEACTIVATE | RT_DEVICE_FLAG_RDCOLLISION);
}

#endif
