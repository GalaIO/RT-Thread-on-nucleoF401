/*
 *	Author:		GalaIO
 *	Date:			2015-8-2 8:19 PM
 *	Description: this function will print a formatted string on the specific device.
 *
 * @param fmt the format
*/
/*@{*/
#include "board.h"
#include "bsp_user.h"
#include "stdio.h"

#define RT_PRINTF_SIZE		128
#define rt_printf_none(buf)

void rt_printf(const char *dev_name,const char *fmt, ...)
{
    char rt_log_buf[RT_PRINTF_SIZE];
    va_list args;
    rt_size_t length;
		rt_device_t	dev = rt_device_find(dev_name);
		if(dev == RT_NULL){
			return ;
		}else{
			if(RT_EOK != rt_device_open(dev,RT_DEVICE_OFLAG_WRONLY)){
				return ;
			}
		}

    va_start(args, fmt);
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the rt_log_buf, we have to adjust the output
     * length. */
    //length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
		//using std C lib, vsnprintf not the rt_vsnprintf.
		//when vsnprintf() generate a sub string, then rt_hw_console_output() it.
		//it's more fast, then putc by putc.
		length = vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > RT_CONSOLEBUF_SIZE - 1)
        length = RT_CONSOLEBUF_SIZE - 1;
#ifdef RT_USING_DEVICE
			do{
        rt_uint16_t old_flag = dev->flag;

        dev->flag |= RT_DEVICE_FLAG_STREAM;
        rt_device_write(dev, 0, rt_log_buf, length);
        dev->flag = old_flag;
			}while(0);
#else
    rt_printf_none(rt_log_buf);
#endif
    va_end(args);
		
		rt_device_close(dev);
}

void rt_printff(rt_device_t device,const char *fmt, ...)
{
    va_list args;
    rt_size_t length;
    static char rt_log_buf[RT_PRINTF_SIZE];

    RT_ASSERT(device != RT_NULL);
    va_start(args, fmt);
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the rt_log_buf, we have to adjust the output
     * length. */
    //length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
		//using std C lib, vsnprintf not the rt_vsnprintf.
		//when vsnprintf() generate a sub string, then rt_hw_console_output() it.
		//it's more fast, then putc by putc.
		length = vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > RT_CONSOLEBUF_SIZE - 1)
        length = RT_CONSOLEBUF_SIZE - 1;
#ifdef RT_USING_DEVICE
			do{
        rt_uint16_t old_flag = device->flag;

        device->flag |= RT_DEVICE_FLAG_STREAM;
        rt_device_write(device, 0, rt_log_buf, length);
        device->flag = old_flag;
			}while(0);
#else
    rt_printf_none(rt_log_buf);
#endif
    va_end(args);
		
}

void rt_putBuf(const char *dev_name,const char *str, const int length)
{
		rt_device_t	dev = rt_device_find(dev_name);
		if(dev == RT_NULL){
			return ;
		}else{
			if(RT_EOK != rt_device_open(dev,RT_DEVICE_OFLAG_WRONLY)){
				return ;
			}
		}
#ifdef RT_USING_DEVICE
			do{
        rt_uint16_t old_flag = dev->flag;

        dev->flag |= RT_DEVICE_FLAG_STREAM;
        rt_device_write(dev, 0, str, length);
        dev->flag = old_flag;
			}while(0);
#else
    rt_printf_none(rt_log_buf);
#endif
		
		rt_device_close(dev);
}

void rt_putBuff(rt_device_t dev,const char *str, const int length)
{
		
			RT_ASSERT(dev != RT_NULL);
#ifdef RT_USING_DEVICE
			do{
        rt_uint16_t old_flag = dev->flag;

        dev->flag |= RT_DEVICE_FLAG_STREAM;
        rt_device_write(dev, 0, str, length);
        dev->flag = old_flag;
			}while(0);
#else
    rt_printf_none(rt_log_buf);
#endif
		
}
