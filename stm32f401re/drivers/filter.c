/*
 *	Author:		GalaIO
 *	Date:			2015-8-10 10:23 PM
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

#include "filter.h"

/*
 *A、方法： 
取a=0~1
本次滤波结果=（1-a）*上次滤波结果+a*本次采样值

B、优点：
对周期性干扰具有良好的抑制作用
适用于波动频率较高的场合
C、缺点：
相位滞后，灵敏度低
滞后程度取决于a值大小
不能消除滤波频率高于采样频率的1/2的干扰信号

D、a的选取，设滤波时间为t，采样频率为F则a＝1/tF#

first ord lag filter.
xfol = numpy.zeros(sz)
#a must 0<a<1
#如果a越小，数据曲线滞后越严重，同时数据曲线越平滑；
#a越大，滤波曲线越接近真实数据，但是滤波效果越差。
a = 0.09
for i in range(1,n_iter):
    #测量值去a的权，加上累积量*（1-a）的权值，得到信息滤波值.
    xfol[i] = a*z[i] + (1-a)*xfol[i-1]
**/

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
float fol_filter(float K,float last,float curr){
		return (K*curr+(1-K)*last);
}


/*
 *4、递推平均滤波法（又称滑动平均滤波法）
A、方法：
   把连续取N个采样值看成一个队列
   队列的长度固定为N
   每次采样到一个新数据放入队尾,并扔掉原来队首的一次数据.(先进先出原则)
   把队列中的N个数据进行算术平均运算,就可获得新的滤波结果
   N值的选取：流量，N=12；压力：N=4；液面，N=4~12；温度，N=1~4
B、优点：
   对周期性干扰有良好的抑制作用，平滑度高
   适用于高频振荡的系统 
C、缺点：
   灵敏度低
   对偶然出现的脉冲性干扰的抑制作用较差
   不易消除由于脉冲干扰所引起的采样值偏差
   不适用于脉冲干扰比较严重的场合
   比较浪费RAM
	 #递推均值滤波
	xlf = numpy.zeros(sz)
	#选择FIFO中的数据长度，N越大越平滑。
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
#二阶比一阶可调参数增多，也越容易控制。
#如果TC越小，越接近数据；越大平滑越厉害；tc<1
tc = 0.001
#L对数据产生影响很小，超级小。
L = 1000
#如果R越小，越接近数据；越大平滑越厉害；R>1
R = 30000
for i in range(2,n_iter):
    xsol[i] = (z[i] + tc*(2*L+R)*xsol[i-1] - tc*L*xsol[i-2])/(tc*(L+R)+1)
    #xsol[i] = (z[i] + tc*R*xsol[i-1] - tc*xsol[i-2])/(tc*R+1)
**/

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
 *卡尔曼滤波是一种高效率的递归滤波器（ 自 回归滤波器） ， 它能够从一系列的不完全及包含噪声的测量中，
估计动态系统的状态。
#klman 滤波效果可以接近任一滤波。
#klman filter.

以下的卡尔曼程序是对一个随机常数进行滤波，其具有以下特性。
		一般简单模型滤波器方程和参数，即x不随时间发生状态变化，都是观测误差，
		然后没有外部激励，只有一个维度的估计值，同时测量误差服从正态分布，那么就是下面这个方程。

#过程激励噪声方差 Q,但是一个小的非零常数可以方便地调整滤波器参数
Q = 1e-6 # process variance

#对于固定测量方差R值来说，表示整体数据的方差，所以方差越小，采集数据与真实数据差距也越小，那么kalman滤波曲线越接近真实数据曲线。
#越大，越平滑。
R = 0.03**2 # estimate of measurement variance, change to see effect

# intial guesses
xhat[0] = 0.0
#如果P不为1，比较好。
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
 *@Param Q,	过程激励噪声方差 Q,但是一个小的非零常数可以方便地调整滤波器参数
 *@Param R,	固定测量方差R值来说，表示整体数据的方差，所以方差越小，采集数据与真实数据差距也越小
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

