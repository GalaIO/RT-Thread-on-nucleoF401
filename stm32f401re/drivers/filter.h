/*
 *	Author:		GalaIO
 *	Date:		2015-8-10 10:23 PM
 *	Description: 
 *			---add a ��һ���ͺ��˲��㷨��The first-order lag filtering method.
 *				--fol_filter(k,v)
 *			---add Recursion average filtering method.
 *				--ra_filter(fifo,v)
 *			---Klman Filter for single Variable.
 *				--kls_filter(Q,R,K,c,X,P)
 *			---a �������ͺ��˲��㷨��The second-order lag filtering method.
 *				--sol_filter(TC,L,R,curr,last,llast)
 *			---���Ǳ�֤���е��˲��������ǿ�����ģ����ڲ���ϵͳ�����£�û��ȫ�ֱ���.
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
 *a ��һ���ͺ��˲��㷨��The first-order lag filtering method.
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
 *@Param Q,	���̼����������� Q,����һ��С�ķ��㳣�����Է���ص����˲�������
 *@Param R,	�̶���������Rֵ��˵����ʾ�������ݵķ�����Է���ԽС���ɼ���������ʵ���ݲ��ҲԽС
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
 *a �������ͺ��˲��㷨��The second-order lag filtering method.
 *
 *@Param TC,	�˲�ʱ��T * ����C,  ���TCԽС��Խ�ӽ����ݣ�Խ��ƽ��Խ������tc<1
 *@Param L,		�˲����ֵ���������˲�Ӱ���С
 *@Param R,		�˲�����ֵ�����RԽС��Խ�ӽ����ݣ�Խ��ƽ��Խ������R>1
 *@Param curr,	the current data.
 *@Param last,	the last data.
 *@Param llast,	the last's last data.
 *
 *@retrun, return the Filter result.
 *
**/
float sol_filter(float TC,int L,int R,float curr,float last,float llast);

#endif
