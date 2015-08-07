/*
 *	Author:		GalaIO
 *	Date:			2015-8-2 8:19 PM
 *	Description: this function will scanf a formatted string on the specific device.
 *
 * @param fmt the format
*/
/*@{*/
#include "board.h"
#include "bsp_user.h"
#include <string.h>
#include "stdio.h"
#include <ctype.h>
#include <stdlib.h>


int _doscan(rt_device_t dev, const char *format, va_list ap);
/*
 *the scanf get the format string from the device.
 *
 *@param device, get a buffer from it.
 *@param buf, get a buffer from it.
 *@param length, get the length of buf.
 *
 *@return the the length of string (no including the '\0').
 *
**/
/*int rt_scanf(const char *dev_name,const char *format, ...)
{
    va_list ap;
    int retval;
		rt_device_t	dev = rt_device_find(dev_name);
		if(dev == RT_NULL){
			return EOF;
		}else{
			if(RT_EOK != rt_device_open(dev,RT_DEVICE_OFLAG_RDONLY)){
				return EOF;
			}
		}

    va_start(ap, format);

    retval = _doscan(dev, format, ap);

    va_end(ap);
		
		//close the device.
		rt_device_close(dev);
		
    return retval;
}*/


/*
 *get a char from the certain device.
 *
 *@param device, get a char from it.
 *
 *@return the getted char.
 *
**/
int8_t rt_getc(rt_device_t device){
		int8_t ch; 
    RT_ASSERT(device != RT_NULL);
		/*@added GalaIO, release the rx_sem*/
		if(rt_device_Rx_Wait(device) != RT_EOK){
			return 0;
		}else{
			if(rt_device_read(device,0,&ch,1) == 1)
				return ch;
			else
				return 0;
		}
		
}

/*
 *get a string from the certain device.
 *
 *@param device, get a string from it.
 *@param str, 	 store the data.
 *
 *@return the the length of string (no including the '\0').
 *
**/
int8_t rt_getss(rt_device_t device, char *str){
		uint8_t i = 0; 
    RT_ASSERT(device != RT_NULL);
		do{
			if(rt_device_Rx_Wait(device) != RT_EOK){
					str[i] = '\0';
					return i;
			}else{
				if(rt_device_read(device,0,str + i,1) != 1){
					str[i] = '\0';
					return i;
				}
				i++;
			}
		//the end of string.
		}while(str[i-1] != '\0');
		return i-1;	
}
/*
 *get a buffer from the certain device.
 *
 *@param device, get a buffer from it.
 *@param buf, get a buffer from it.
 *@param length, get the length of buf.
 *
 *@return the the length of string (no including the '\0').
 *
**/
int8_t rt_getBuff(rt_device_t device, char *buf, int length){
		uint8_t i = 0; 
		int 		temp;
    RT_ASSERT(device != RT_NULL);
		while(length){
			if(rt_device_Rx_Wait(device) != RT_EOK){
					buf[i] = '\0';
					return i;
			}else{
				if((temp = rt_device_read(device,0,buf + i,length)) <= 0){
					buf[i] = '\0';
					return i;
				}
				i += temp;
				length -= temp;
			}		
		}
		buf[i] = '\0';
		return i;
}

/*
 *get a string from the certain device name.
 *
 *@param device, get a string from it.
 *@param str, 	 store the data.
 *
 *@return the the length of string (no including the '\0').
 *
**/
int8_t rt_gets(const char *dev_name, char *str){
		uint8_t i = 0; 
		rt_device_t	device = rt_device_find(dev_name);
		if(device == RT_NULL){
			return 0;
		}else{
			if(RT_EOK != rt_device_open(device,RT_DEVICE_OFLAG_RDONLY)){
				return 0;
			}
		}
		do{
			if(rt_device_Rx_Wait(device) != RT_EOK){
					str[i] = '\0';
					rt_device_close(device);
					return i;
			}else{
				if(rt_device_read(device,0,str + i,1) != 1){
					str[i] = '\0';
					rt_device_close(device);
					return i;
				}
				i++;
			}
		//the end of string.
		}while(str[i-1] != '\0');	
		rt_device_close(device);
		return i-1;	
}
/*
 *get a buffer from the certain device.
 *
 *@param device, get a buffer from it.
 *@param buf, get a buffer from it.
 *@param length, get the length of buf.
 *
 *@return the the length of string (no including the '\0').
 *
**/
int8_t rt_getBuf(const char *dev_name, char *buf, int length){
		uint8_t i = 0; 
		int 		temp;
		rt_device_t	device = rt_device_find(dev_name);
		if(device == RT_NULL){
			return 0;
		}else{
			if(RT_EOK != rt_device_open(device,RT_DEVICE_OFLAG_RDONLY)){
				return 0;
			}
		}
		while(length){
			if(rt_device_Rx_Wait(device) != RT_EOK){
					buf[i] = '\0';
					rt_device_close(device);
					return i;
			}else{
				if((temp = rt_device_read(device,0,buf + i,length)) <= 0){
					buf[i] = '\0';
					rt_device_close(device);
					return i;
				}
				i += temp;
				length -= temp;
			}		
		}
		buf[i] = '\0';
		rt_device_close(device);
		return i;
}

