/*
*	Author: 		laoguo
*   Date:   		2014-11-16-14:04
*	Deline:			Fifo_Auto �ĺ꺯����c���� ���汾   �޸�ָ��汾 
*	Version:		1.2.0
*   Description: 	Auto Fifo������  ������ƽʱ��ѭ�����е������� ����������
*					Ѱ����ѭ������һ�� ��� ���ӣ�ͬʱ����ԭ�����ݣ���������ʱ���������
*					��Auto Fifo�ڶ�����ʱ Ҳ������ӣ������ᶪ��һЩ���ݣ��ڶ���ʱ��Խ�� Խ���ױ����� 
*/
#ifndef _FIFO_AUTO_2_H

#define _FIFO_AUTO_2_H

typedef struct _Fifo_Auto{

	void * data;
	short  head; //16λ  -256-256
	short  tail;
	short  length;
	short  maxSize;

}FIFO_str,*FIFO_ptr;

//�����ȳ�ʼ�� 
#define FIFO_INIT(fifo,ptr,max_size){\
	fifo->tail=-1;\
	fifo->head=-1;\
	fifo->length=0;\
	fifo->data=ptr;\
	fifo->maxSize=max_size;\
	}

#define  FIFO_LEN(fifo)    (fifo->length)

#define  FIFO_MAX(fifo)    (fifo->maxSize)

#define FIFO_FULL(fifo)    (FIFO_LEN(fifo)>=FIFO_MAX(fifo))

#define FIFO_EMPTY(fifo)   (FIFO_LEN(fifo)<=0)

#define  FIFO_GET(fifo)     (FIFO_EMPTY(fifo)==0?((char *)fifo->data)[(fifo->tail+1)%fifo->maxSize]:-1)

#define  FIFO_GET_INT(fifo) (FIFO_EMPTY(fifo)==0?((int  *)fifo->data)[(fifo->tail+1)%fifo->maxSize]:-1)

//������ ֱ�ӻ�� fifo��ָ�� �����жϱ߽�  ���� ��� ����   ͬʱ ����ע��fifo��ѭ�����У���ͷβ ָ�����ԭ�� ��ֹ����Խ�� 
#define  FIFO_PTR(fifo)     (FIFO_EMPTY(fifo)==0?&(((char *)fifo->data)[(fifo->tail+1)%fifo->maxSize]):-1)

#define  FIFO_PTR_INT(fifo) (FIFO_EMPTY(fifo)==0?&(((int  *)fifo->data)[(fifo->tail+1)%fifo->maxSize]):-1)

extern void  FIFO_IN(FIFO_ptr fifo,char ch);

extern void  FIFO_IN_INT(FIFO_ptr fifo,int ch);

	
extern void  FIFO_IN_AUTO(FIFO_ptr fifo,char ch);

extern void  FIFO_IN_INT_AUTO(FIFO_ptr fifo,int ch); 

extern char  FIFO_OUT(FIFO_ptr fifo);  

extern int   FIFO_OUT_INT(FIFO_ptr fifo);

#endif


