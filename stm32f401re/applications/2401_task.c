
#include "rtuser.h"

#ifdef RT_USING_NRF2401

#include "bsp_24l01.h"
#include "string.h"

char tmp_data[32]="hello world!!";

void rt_thread_entry_2401(void* parameter){
/*		
#if 1
	
		//set 2401 as Tx mode.
		NRF24L01_TX_Mode();
		rt_kprintf("-->2401thread: set tx mode!\r\n");
		rt_thread_delay(1);
		while(1){
			if(TX_OK != NRF24L01_TxPacket((u8 *)tmp_data)){
					rt_kprintf("-->2401thread: tx Error!\r\n");
			}else{
					rt_kprintf("-->2401thread: tx OK!\r\n");
			}
			rt_thread_delay(500);
			
		}
#else
		//set 2401 as Rx mode.
		NRF24L01_RX_Mode();
		rt_kprintf("-->2401thread: set rx mode!\r\n");
		rt_thread_delay(1);
		while(1){
			if(NRF24L01_RxPacket((u8 *)tmp_data)){
					//rt_kprintf("-->2401thread: rx Error!\r\n");
			}else{
					rt_kprintf("-->2401thread: rx OK!\r\n");
					rt_kprintf("-->2401thread: %s\r\n",tmp_data);
			}
			rt_thread_delay(100);
			
		}
#endif*/

		//device operate!!
		
		rt_device_t nrf2401_dev = rt_device_find_open("2401",'a');
		while(1){
#if 0
				rt_device_read(nrf2401_dev,0,tmp_data,14);
				tmp_data[14] = 0;
				rt_kprintf("%s\r\n",tmp_data);
				rt_device_read(nrf2401_dev,0,tmp_data,5);
				tmp_data[5] = 0;
				rt_kprintf("%s\r\n",tmp_data);
#else
				//rt_device_write(nrf2401_dev,0,tmp_data,strlen(tmp_data));
				rt_printff(nrf2401_dev,"hello world %d!!",12);
#endif
				rt_thread_delay(20);
		}
	
}

#endif
