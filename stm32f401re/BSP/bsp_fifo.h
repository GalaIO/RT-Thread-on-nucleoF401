/*
*	Author: 		laoguo
*   Date:   		2014-11-16-14:04
*	Deline:			Fifo_Auto 的宏函数和c函数 混编版本   修改指针版本 
*	Version:		1.2.0
*   Description: 	Auto Fifo的作用  区别于平时的循环队列的作用是 她不仅可以
*					寻常的循环队列一样 入队 出队，同时保护原有数据，当队列满时，不能入队
*					而Auto Fifo在队列满时 也可以入队，不过会丢掉一些数据，在队列时间越长 越容易被丢弃 
*/
#ifndef _FIFO_AUTO_2_H

#define _FIFO_AUTO_2_H

typedef struct _Fifo_Auto{

	void * data;
	short  head; //16位  -256-256
	short  tail;
	short  length;
	short  maxSize;

}FIFO_str,*FIFO_ptr;

//必须先初始化 
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

//在这里 直接获得 fifo的指针 必须判断边界  否则 后果 不堪   同时 必须注意fifo（循环队列）的头尾 指针操作原理 防止数组越界 
#define  FIFO_PTR(fifo)     (FIFO_EMPTY(fifo)==0?&(((char *)fifo->data)[(fifo->tail+1)%fifo->maxSize]):-1)

#define  FIFO_PTR_INT(fifo) (FIFO_EMPTY(fifo)==0?&(((int  *)fifo->data)[(fifo->tail+1)%fifo->maxSize]):-1)

extern void  FIFO_IN(FIFO_ptr fifo,char ch);

extern void  FIFO_IN_INT(FIFO_ptr fifo,int ch);

	
extern void  FIFO_IN_AUTO(FIFO_ptr fifo,char ch);

extern void  FIFO_IN_INT_AUTO(FIFO_ptr fifo,int ch); 

extern char  FIFO_OUT(FIFO_ptr fifo);  

extern int   FIFO_OUT_INT(FIFO_ptr fifo);

#endif


