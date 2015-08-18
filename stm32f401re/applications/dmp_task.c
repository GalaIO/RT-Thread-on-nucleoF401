
#include "rtuser.h"

#ifdef RT_USING_MPU6050
#error 8888
#include "mpu6050_dmp.h"

#include "bsp_gpio.h"

#include "finsh.h"

#include "board.h"

#include "filter.h"

#include "math.h"

#define BYTE3(tmp)	(((tmp)>>24)&0XFF)
#define BYTE2(tmp)	(((tmp)>>16)&0XFF)
#define BYTE1(tmp)	(((tmp)>>8)&0XFF)
#define BYTE0(tmp)	((tmp)&0XFF)

void nmZT_Send(char *data_to_send,int *length,float rol,float pit,float yaw,int Alt)
{
	u8 _cnt=0;
	u8 i;
	u8 sum = 0;
	vs16 _temp;
	vs32 _temp2 = Alt;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	_temp = (int)(rol*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(pit*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
		
	/*if(Rc_C.ARMED==0)		data_to_send[_cnt++]=0xA0;	
	else if(Rc_C.ARMED==1)		*/data_to_send[_cnt++]=0xA1;
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;

	//Uart1_Put_Buf(data_to_send,_cnt);
	*length = _cnt;
}

void nm6ZData_Send(char *data_to_send,int *length,float accx,float accy,float accz
	,float gyrox,float gyroy,float gyroz)
{
	u8 _cnt=0;
	u8 i,sum = 0;
	vs16 _temp;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	_temp = (short)(accx*100);
	data_to_send[_cnt++]=BYTE1(_temp); 
	data_to_send[_cnt++]=BYTE0(_temp); 
	_temp = (short)(accy*100);
	data_to_send[_cnt++]=BYTE1(_temp); 
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (short)(accz*100);
	data_to_send[_cnt++]=BYTE1(_temp); 
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (short)(gyrox*100);
	data_to_send[_cnt++]=BYTE1(_temp); 
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (short)(gyroy*100);
	data_to_send[_cnt++]=BYTE1(_temp); 
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (short)(gyroz*100);
	data_to_send[_cnt++]=BYTE1(_temp); 
	data_to_send[_cnt++]=BYTE0(_temp);
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;

	//Uart1_Put_Buf(data_to_send,_cnt);
	*length = _cnt;
}
void nm6ZData_SendXYZ(char *data_to_send,int *length,int accx,int accy,int accz
	,int gyrox,int gyroy,int gyroz)
{
	u8 _cnt=0;
	u8 i,sum = 0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(accx); 
	data_to_send[_cnt++]=BYTE0(accx); 
	data_to_send[_cnt++]=BYTE1(accy); 
	data_to_send[_cnt++]=BYTE0(accy);
	data_to_send[_cnt++]=BYTE1(accz); 
	data_to_send[_cnt++]=BYTE0(accz);
	data_to_send[_cnt++]=BYTE1(gyrox); 
	data_to_send[_cnt++]=BYTE0(gyrox);
	data_to_send[_cnt++]=BYTE1(gyroy); 
	data_to_send[_cnt++]=BYTE0(gyroy);
	data_to_send[_cnt++]=BYTE1(gyroz); 
	data_to_send[_cnt++]=BYTE0(gyroz);
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;

	//Uart1_Put_Buf(data_to_send,_cnt);
	*length = _cnt;
}


char buf[50];
int  length;
#if defined(RT_USING_DMP)
DMP_t dmp;
#endif
float acce[3];
float gyro[3];
//int acce[3];
//int gyro[3];

//一阶滤波结构体
//该K参数对较慢的拨动有较好的滤波，对于快速频率的不行。
//也就是说现在滤波数据对姿态不敏感，
fol_filter_t folx = { 
	0.35,	//K.
	1	 	//Last.
};
fol_filter_t foly = { 
	0.35,	//K.
	1	 	//Last.
};
fol_filter_t folz = { 
	0.35,	//K.
	1	 	//Last.
};

void setFol_filter(int KK){
	rt_base_t	level;
	float K = 1.0*KK/1000;
	/* disable interrupt ,enter the critical code area, can't be interrupted.*/
	level = rt_hw_interrupt_disable();
	folx.K = K;
	foly.K = K;
	folz.K = K;
	/* enable interrupt, leaving the critical code area, can't be interrupted.*/
	rt_hw_interrupt_enable(level);
}
#ifdef RT_USING_FINSH
	FINSH_FUNCTION_EXPORT(setFol_filter,set fol_filters K);
#endif

void rt_thread_entry_dmp(void* parameter)
{
		rt_device_t log_dev = rt_device_find_open("uart1",'a');
		rt_device_t dmp_dev = rt_device_find_open(DMP_DEVICE_NAME,'r');
		RT_ASSERT(log_dev != RT_NULL && dmp_dev != RT_NULL);
	
		rt_printff(log_dev,"%s","run in dmp_thread!!!\r\n");
	
    while (1)
    {
#if defined(RT_USING_DMP)
			//read ZT data from device, and send it.
			if(rt_device_read(dmp_dev,0,&dmp,sizeof(DMP_t))){
				nmZT_Send(buf,&length,dmp.Roll,dmp.Pitch,dmp.Yaw,0);
				rt_putBuff(log_dev,buf,length);
//			rt_printff(log_dev,"Roll: %.2f\tPitch: %.2f\tYaw: %.2f\r\n",dmp.Roll,dmp.Pitch,dmp.Yaw);
				
				//read acce and gryo from device, and sent it to niming.
				if(rt_device_control(dmp_dev,DMP_CMD_GET_ACCE,acce) == RT_EOK
						&& rt_device_control(dmp_dev,DMP_CMD_GET_GYRO,gyro) == RT_EOK){
					nm6ZData_Send(buf,&length,acce[0],acce[1],acce[2],gyro[0],gyro[1],gyro[2]);
					rt_putBuff(log_dev,buf,length);
				}
			}
#else
			//read acce and gryo from device, and sent it to niming.
				if(rt_device_control(dmp_dev,DMP_CMD_GET_ACCE,acce) == RT_EOK
						&& rt_device_control(dmp_dev,DMP_CMD_GET_GYRO,gyro) == RT_EOK){
					nm6ZData_Send(buf,&length,acce[0],acce[1],acce[2],gyro[0],gyro[1],gyro[2]);
					//一阶原始数据滤波
//					nm6ZData_Send(buf,&length,acce[0],FOL_FILTER(folx,acce[0]),acce[1],FOL_FILTER(foly,acce[1]),acce[2],FOL_FILTER(folz,acce[2]));
//					nm6ZData_Send(buf,&length,gyro[0],FOL_FILTER(folx,gyro[0]),gyro[1],FOL_FILTER(foly,gyro[1]),gyro[2],FOL_FILTER(folz,gyro[2]));
					//一阶姿态滤波
//					folx.last = fol_filter(folx.K,folx.last+(float)0.02*gyro[0],atan(acce[1]/acce[2])*57.32);
//					foly.last = fol_filter(foly.K,foly.last+(float)0.02*gyro[1],atan(acce[0]/acce[2])*57.32);
//					folz.last = fol_filter(folz.K,folz.last,folz.last+(float)0.02*gyro[2]);
//					nmZT_Send(buf,&length,folx.last,-foly.last,folz.last,0);
					rt_putBuff(log_dev,buf,length);
					//rt_kprintf("%.2f %.2f %.2f %.2f %.2f %.2f\r\n",acce[0],acce[1],acce[2],gyro[0],gyro[1],gyro[2]);
				}
//				if(rt_device_control(dmp_dev,DMP_CMD_GET_ACCEXYZ,acce) == RT_EOK
//						&& rt_device_control(dmp_dev,DMP_CMD_GET_GYROXYZ,gyro) == RT_EOK){
//					nm6ZData_SendXYZ(buf,&length,acce[0],acce[1],acce[2],gyro[0],gyro[1],gyro[2]);
//					rt_putBuff(log_dev,buf,length);
//				}
			rt_thread_delay(20);
#endif
    }
}

#endif
