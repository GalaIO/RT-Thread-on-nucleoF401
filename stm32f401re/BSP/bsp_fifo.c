/*
*	Author: 		laoguo
*   Date:   		2014-11-16-14:04
*	Deline:			Fifo_Auto 的宏函数和c函数 混编版本   修改指针版本 
*	Version:		1.2.0
*   Description: 	Auto Fifo的作用  区别于平时的循环队列的作用是 她不仅可以
*					寻常的循环队列一样 入队 出队，同时保护原有数据，当队列满时，不能入队
*					而Auto Fifo在队列满时 也可以入队，不过会丢掉一些数据，在队列时间越长 越容易被丢弃 
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
