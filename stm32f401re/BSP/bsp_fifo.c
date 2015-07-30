/*
*	Author: 		laoguo
*   Date:   		2014-11-16-14:04
*	Deline:			Fifo_Auto �ĺ꺯����c���� ���汾   �޸�ָ��汾 
*	Version:		1.2.0
*   Description: 	Auto Fifo������  ������ƽʱ��ѭ�����е������� ����������
*					Ѱ����ѭ������һ�� ��� ���ӣ�ͬʱ����ԭ�����ݣ���������ʱ���������
*					��Auto Fifo�ڶ�����ʱ Ҳ������ӣ������ᶪ��һЩ���ݣ��ڶ���ʱ��Խ�� Խ���ױ����� 
*/
#include "bsp_fifo.h"

char FIFO_OUT(FIFO_ptr fifo)  {
	
	if(FIFO_EMPTY(fifo)==0){
		fifo->tail=(fifo->tail+1)%fifo->maxSize;
		fifo->length-=1;
		return  ((char *)fifo->data)[fifo->tail];
	}else{
		return (char)-1;
	}	
	
}   

int  FIFO_OUT_INT(FIFO_ptr fifo) {

	if(FIFO_EMPTY(fifo)==0){
		fifo->tail=(fifo->tail+1)%fifo->maxSize;
		fifo->length-=1;
		return ((int  *)fifo->data)[fifo->tail];
	}else{
		
		return -1;
		
	}

}


void FIFO_IN(FIFO_ptr fifo,char ch) {
	if(!FIFO_FULL(fifo)){
		fifo->head=(fifo->head+1)%fifo->maxSize;
		((char *)fifo->data)[fifo->head]=(char)ch;
		fifo->length++;
	}
}

void  FIFO_IN_INT(FIFO_ptr fifo,int ch) {
	if(!FIFO_FULL(fifo)){
		fifo->head=(fifo->head+1)%fifo->maxSize;
		((int *)fifo->data)[fifo->head]=(int)ch;
		fifo->length++;
	}
}

	
void  FIFO_IN_AUTO(FIFO_ptr fifo,char ch) {
	if(!FIFO_FULL(fifo)){
		fifo->head=(fifo->head+1)%fifo->maxSize;
		((char *)fifo->data)[fifo->head]=(char)ch;
		fifo->length++;
	}else{
		FIFO_OUT(fifo);
		fifo->head=(fifo->head+1)%fifo->maxSize;
		((char *)fifo->data)[fifo->head]=(char)ch;
		fifo->length++;
	}
}

void  FIFO_IN_INT_AUTO(FIFO_ptr fifo,int ch) {
	if(!FIFO_FULL(fifo)){
		fifo->head=(fifo->head+1)%fifo->maxSize;
		((int *)fifo->data)[fifo->head]=(int)ch;
		fifo->length++;
	}else{
		FIFO_OUT_INT(fifo);
		fifo->head=(fifo->head+1)%fifo->maxSize;
		((int *)fifo->data)[fifo->head]=(int)ch;
		fifo->length++;
	}
}
