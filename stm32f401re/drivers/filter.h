/*
 *	Author:		GalaIO
 *	Date:		2015-8-10 10:23 PM
 *	Description: 
 *			---add a （一阶滞后滤波算法）The first-order lag filtering method.
 *				--fol_filter(k,v)
 *			---add Recursion average filtering method.
 *				--ra_filter(fifo,v)
 *			---Klman Filter for single Variable.
 *				--kls_filter(Q,R,K,c,X,P)
 *			---a （二阶滞后滤波算法）The second-order lag filtering method.
 *				--sol_filter(TC,L,R,curr,last,llast)
 *			---我们保证所有的滤波函数都是可重入的，即在操作系统环境下，没有全局变量.
 *
**/

#ifndef _FILTER_H_
#define _FILTER_H_

#include "bsp_user.h"
#include "board.h"
#include "GFifo.h"

/*
 *a struct for fol filter.
**/
typedef struct {
	float K;
	float last;
}fol_filter_t;
#define FOL_FILTER(filter,curr)		(filter.last=fol_filter(filter.K,filter.last,curr),filter.last)
/*
 *a （一阶滞后滤波算法）The first-order lag filtering method.
 *
 *@param k, the lag Coefficient.
 *@param last, the last value for filter.
 *@param curr, current data.
 *
 *@return the filter result.
 *
**/
float fol_filter(float K,float last,float curr);


/*
 *a struct for fol Recursion average filter.
**/
typedef struct {
	GFifo_t fifo;
	float 	sum;
}ra_filter_t;
#define RA_FILTER(filter,curr)	(ra_filter(&(filter.fifo),curr,&(filter.sum)))
/*
 * a Recursion average filtering method. smooth the data.
 *
 *@param fifo,	the handler of the fifo-data, stroed the data in FIFO.
 *@param curr,	the current new data inputed.
 *@param sum,		a handler with the real-time summary, you should init it with 0 value.
 *
 *@return,	return the current smooth result.
 *
 *@notes, add sum for speeding up the process.
 *
**/

float ra_filter(GFifo_t *fifo,float curr,float *sum);


/*
 *a struct for fol Klman Filter.
**/
typedef struct {
	float 	Q;
	float 	R;
	float 	X;
	float 	P;
}kls_filter_t;
#define KLS_FILTER(filter,curr)		(kls_filter(filter.Q,filter.R,curr,&(filter.X),&(filter.P)))
/*
 *a Klman Filter for single Variable.
 *
 *@Param Q,	过程激励噪声方差 Q,但是一个小的非零常数可以方便地调整滤波器参数
 *@Param R,	固定测量方差R值来说，表示整体数据的方差，所以方差越小，采集数据与真实数据差距也越小
 *@Param curr,	the current data.
 *@param X,	the History of klman filter result.
 *@param P, the history of klman p.
 *
 *@return, the update filtered data. 
 *
**/
float kls_filter(float Q,float R,float curr,float *X,float *P);


/*
 *a struct for fol Klman Filter.
**/
typedef struct {
	float 	TC;
	int 	L;
	int 	R;
	float 	last;
	float 	llast;
}sol_filter_t;
/*operation struct;*/
float SOL_FILTER(sol_filter_t *filter,float curr);
/*
 *a （二阶滞后滤波算法）The second-order lag filtering method.
 *
 *@Param TC,	滤波时间T * 电容C,  如果TC越小，越接近数据；越大平滑越厉害；tc<1
 *@Param L,		滤波电感值，对数据滤波影响较小
 *@Param R,		滤波电阻值，如果R越小，越接近数据；越大平滑越厉害；R>1
 *@Param curr,	the current data.
 *@Param last,	the last data.
 *@Param llast,	the last's last data.
 *
 *@retrun, return the Filter result.
 *
**/
float sol_filter(float TC,int L,int R,float curr,float last,float llast);

#endif
