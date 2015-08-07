
#include "mpu6050_dmp.h"

#include "rtuser.h"

#include "bsp_gpio.h"

#include "finsh.h"

#include "board.h"
//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void niming_report(u8 fun,u8* data,u8* send_buf,int *length,u8 len)
{
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//��������
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���	
	*length = len + 4;
}
//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
void mpu6050_send_data(u8 *buf,int *length,short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	niming_report(0XA1,tbuf,buf,length,12);//�Զ���֡,0XA1
}	
//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void report_imu(u8 *buf,int *length,short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//��0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	niming_report(0XAF,tbuf,buf,length,28);//�ɿ���ʾ֡,0XAF
} 

#define BYTE3(tmp)	(((tmp)>>24)&0XFF)
#define BYTE2(tmp)	(((tmp)>>16)&0XFF)
#define BYTE1(tmp)	(((tmp)>>8)&0XFF)
#define BYTE0(tmp)	((tmp)&0XFF)

void nmZT_Send_Status(char *data_to_send,int *length,float rol,float pit,float yaw,int Alt)
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

char buf[50];
int  length;
DMP_t dmp;
void rt_thread_entry_dmp(void* parameter)
{
		rt_device_t log_dev = rt_device_find_open("uart1",'a');
		rt_device_t dmp_dev = rt_device_find_open(DMP_DEVICE_NAME,'r');
		RT_ASSERT(log_dev != RT_NULL && dmp_dev != RT_NULL);
	
		rt_printff(log_dev,"%s","run in dmp_thread!!!\r\n");
	
    while (1)
    {
			
			rt_device_read(dmp_dev,0,&dmp,sizeof(DMP_t));
//			report_imu((u8 *)buf,&length,0,0,0,0,0,0,dmp.Roll,dmp.Pitch,dmp.Yaw);
			nmZT_Send_Status(buf,&length,dmp.Roll,dmp.Pitch,dmp.Yaw,0);
			rt_putBuff(log_dev,buf,length);
//			rt_printff(log_dev,"Roll: %.2f\tPitch: %.2f\tYaw: %.2f\r\n",dmp.Roll,dmp.Pitch,dmp.Yaw);
			
			//rt_thread_delay(4);
			
    }
}
