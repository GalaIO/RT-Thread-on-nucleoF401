/*
 * Author: 	    GalaIO
 * Date:   	    2015-8-3 10:05 PM
 * Description: -a macro Fifo lib, for more situation, and FIFO-input has two ways.
 *					--one, you can input normal, but if FIFO is full, not input.
 *				 	--two, when the FIFO is full, then FIFO-output and FIFO-input again.
 *				-support FIFO_TOP, just look the TOP ele, not OUPUT it.
**/

#ifndef _BSP_FIFO3_H_
#define _BSP_FIFO3_H_

//most length of FIFO is 0-65535.
typedef struct __GFifo__{

	void 			*data;
	unsigned short  head; 
	unsigned short  tail;
	unsigned short  length;
	unsigned short  maxSize;

}GFifo_t;

//must call FIFO_INIT firstly.
#define GFIFO_INIT(fifo,ptr,max_size){\
(fifo)->tail=0;\
(fifo)->head=0;\
(fifo)->length=0;\
(fifo)->data=ptr;\
(fifo)->maxSize=max_size;\
}

//you shall call it necessarily.
#define GFIFO_REINIT(fifo){\
(fifo)->tail=0;\
(fifo)->head=0;\
(fifo)->length=0;\
}

#define  GFIFO_LEN(fifo)     			((fifo)->length)

#define  GFIFO_MAX(fifo)     			((fifo)->maxSize)

#define  GFIFO_FULL(fifo)    			(GFIFO_LEN(fifo)>=GFIFO_MAX(fifo))

#define  GFIFO_EMPTY(fifo)   			(GFIFO_LEN(fifo)<=0)

#define  GFIFO_TOP(fifo,type)    	(GFIFO_EMPTY(fifo)==0?((type *)(fifo)->data)[(fifo)->tail]:-1)
/*
#define  GFIFO_OUT(fifo,temp,type)		{if(!GFIFO_EMPTY(fifo)){\
temp = ((type *)(fifo)->data)[(fifo)->tail];\
(fifo)->tail = ((fifo)->tail+1)%(fifo)->maxSize;\
(fifo)->length -= 1;\
}else{\
temp = *(type *)0;\
}\
}*/
#define  GFIFO_OUT(fifo,temp,type)		((!GFIFO_EMPTY(fifo))?\
(temp) = ((type *)(fifo)->data)[(fifo)->tail],\
(fifo)->tail = ((fifo)->tail+1)%(fifo)->maxSize,\
(fifo)->length -= 1,1:0)
/*
#define GFIFO_IN(fifo,temp,type)		{if(!GFIFO_FULL(fifo)){\
((type *)(fifo)->data)[(fifo)->head] = temp;\
(fifo)->head = ((fifo)->head+1)%(fifo)->maxSize;\
(fifo)->length++;\
}\
}*/
#define GFIFO_IN(fifo,temp,type)		((!GFIFO_FULL(fifo))?\
((type *)(fifo)->data)[(fifo)->head] = (temp),\
(fifo)->head = ((fifo)->head+1)%(fifo)->maxSize,\
(fifo)->length += 1,1:0)
/*
#define GFIFO_IN_FORCE(fifo,temp,type)	{if(!GFIFO_FULL(fifo)){\
GFIFO_IN(fifo,temp,type);\
}else{\
FIFO_OUT(fifo);\
GFIFO_IN(fifo,temp,type);\
}\
}*/
#define GFIFO_IN_FORCE(fifo,temp,type)	{if(!GFIFO_FULL(fifo)){\
((type *)(fifo)->data)[(fifo)->head] = (temp);\
(fifo)->head = ((fifo)->head+1)%((fifo)->maxSize);\
(fifo)->length += 1;}else{\
(fifo)->tail = ((fifo)->tail+1)%((fifo)->maxSize);\
((type *)(fifo)->data)[(fifo)->head] = (temp);\
(fifo)->head = ((fifo)->head+1)%((fifo)->maxSize);}}

#endif

