/*
 *	Author:		GalaIO
 *	Date:			2015-8-10 10:23 PM
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

#include "filter.h"

/*
 *A�������� 
ȡa=0~1
�����˲����=��1-a��*�ϴ��˲����+a*���β���ֵ

B���ŵ㣺
�������Ը��ž������õ���������
�����ڲ���Ƶ�ʽϸߵĳ���
C��ȱ�㣺
��λ�ͺ������ȵ�
�ͺ�̶�ȡ����aֵ��С
���������˲�Ƶ�ʸ��ڲ���Ƶ�ʵ�1/2�ĸ����ź�

D��a��ѡȡ�����˲�ʱ��Ϊt������Ƶ��ΪF��a��1/tF#

first ord lag filter.
xfol = numpy.zeros(sz)
#a must 0<a<1
#���aԽС�����������ͺ�Խ���أ�ͬʱ��������Խƽ����
#aԽ���˲�����Խ�ӽ���ʵ���ݣ������˲�Ч��Խ�
a = 0.09
for i in range(1,n_iter):
    #����ֵȥa��Ȩ�������ۻ���*��1-a����Ȩֵ���õ���Ϣ�˲�ֵ.
    xfol[i] = a*z[i] + (1-a)*xfol[i-1]
**/

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
float fol_filter(float K,float last,float curr){
		return (K*curr+(1-K)*last);
}


/*
 *4������ƽ���˲������ֳƻ���ƽ���˲�����
A��������
   ������ȡN������ֵ����һ������
   ���еĳ��ȹ̶�ΪN
   ÿ�β�����һ�������ݷ����β,���ӵ�ԭ�����׵�һ������.(�Ƚ��ȳ�ԭ��)
   �Ѷ����е�N�����ݽ�������ƽ������,�Ϳɻ���µ��˲����
   Nֵ��ѡȡ��������N=12��ѹ����N=4��Һ�棬N=4~12���¶ȣ�N=1~4
B���ŵ㣺
   �������Ը��������õ��������ã�ƽ���ȸ�
   �����ڸ�Ƶ�񵴵�ϵͳ 
C��ȱ�㣺
   �����ȵ�
   ��żȻ���ֵ������Ը��ŵ��������ýϲ�
   �������������������������Ĳ���ֵƫ��
   ��������������űȽ����صĳ���
   �Ƚ��˷�RAM
	 #���ƾ�ֵ�˲�
	xlf = numpy.zeros(sz)
	#ѡ��FIFO�е����ݳ��ȣ�NԽ��Խƽ����
	N = 50
**/

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

float ra_filter(GFifo_t *fifo,float curr,float *sum){
		
		if(GFIFO_FULL(fifo)){
				*sum += curr - GFIFO_TOP(fifo,float);
				GFIFO_IN_FORCE(fifo,curr,float);
		}else{
				*sum += curr;
				GFIFO_IN(fifo,curr,float);
		}
		return (*sum/GFIFO_LEN(fifo));
}

/*
 *#second ord lag filter.
xsol = numpy.zeros(sz)
xsol[0] = z[0]
xsol[1] = z[1]
#���ױ�һ�׿ɵ��������࣬ҲԽ���׿��ơ�
#���TCԽС��Խ�ӽ����ݣ�Խ��ƽ��Խ������tc<1
tc = 0.001
#L�����ݲ���Ӱ���С������С��
L = 1000
#���RԽС��Խ�ӽ����ݣ�Խ��ƽ��Խ������R>1
R = 30000
for i in range(2,n_iter):
    xsol[i] = (z[i] + tc*(2*L+R)*xsol[i-1] - tc*L*xsol[i-2])/(tc*(L+R)+1)
    #xsol[i] = (z[i] + tc*R*xsol[i-1] - tc*xsol[i-2])/(tc*R+1)
**/

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
float sol_filter(float TC,int L,int R,float curr,float last,float llast){
	return ((curr + TC*(2*L+R)*last - TC*L*llast)/(TC*(L+R)+1));
}
/*operation struct;*/
float SOL_FILTER(sol_filter_t *filter,float curr){
	
	float temp;
	temp = sol_filter(filter->TC,filter->L,filter->R,curr,filter->last,filter->llast);
	filter->llast = filter->last;
	filter->last = temp;
	
	return temp;
	
}
/*
 *�������˲���һ�ָ�Ч�ʵĵݹ��˲����� �� �ع��˲����� �� ���ܹ���һϵ�еĲ���ȫ�����������Ĳ����У�
���ƶ�̬ϵͳ��״̬��
#klman �˲�Ч�����Խӽ���һ�˲���
#klman filter.

���µĿ����������Ƕ�һ��������������˲���������������ԡ�
		һ���ģ���˲������̺Ͳ�������x����ʱ�䷢��״̬�仯�����ǹ۲���
		Ȼ��û���ⲿ������ֻ��һ��ά�ȵĹ���ֵ��ͬʱ������������̬�ֲ�����ô��������������̡�

#���̼����������� Q,����һ��С�ķ��㳣�����Է���ص����˲�������
Q = 1e-6 # process variance

#���ڹ̶���������Rֵ��˵����ʾ�������ݵķ�����Է���ԽС���ɼ���������ʵ���ݲ��ҲԽС����ôkalman�˲�����Խ�ӽ���ʵ�������ߡ�
#Խ��Խƽ����
R = 0.03**2 # estimate of measurement variance, change to see effect

# intial guesses
xhat[0] = 0.0
#���P��Ϊ1���ȽϺá�
P[0] = 1.0

for k in range(1,n_iter):
    # time update
    xhatminus[k] = xhat[k-1]
    Pminus[k] = P[k-1]+Q

    # measurement update
    K[k] = Pminus[k]/( Pminus[k]+R )
    xhat[k] = xhatminus[k]+K[k]*(z[k]-xhatminus[k])
    P[k] = (1-K[k])*Pminus[k]

**/
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
float kls_filter(float Q,float R,float curr,float *X,float *P){
	
	//time update
	float Xminus = *X;
	float Pminus = *P+Q;
	
	//measurement uodate
	float K = Pminus / (Pminus + R);
	*X = Xminus+K*(curr-Xminus);
	*P = (1-K)*Pminus;
	
	return *X;
	
}

