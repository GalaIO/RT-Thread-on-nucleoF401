
#include "rtuser.h"

#ifdef RT_USING_ENC28J60

#include "bsp_enc28j60.h"


void rt_thread_entry_enc28j60(void* parameter){
	
	while(1){
		
		//query enc data.
		ENC28J60_queryData();
		rt_thread_delay(2);
		
	}
	
}

#endif 
