/*
 *
 *Author:				GalaIO
 *Date:					2015-8-18 10:21 AM
 *Description:	-add two PID prototype. 
 *								-add a location pattern PID.
 *								-add a increment pattern PID.
 *							-add two improved PID.
 *								-add a PID with a dead zone and the variable integral in time-effect.
 *								-add a PID with a dead zone and the variable integral in time-bias-effect.
 *
 *	Notes:here, we quote the python norm, __var mean a private variable, var mean a public variable.
 *				在这里，我们引入了一些Python的规范，当__var一个变量以两个下划线开头时，意味着其是结构体中的
 *				私有变量；而var的寻常用法表示公共变量，值得说明的是该规则当然旨在struct描述的结构体中有效。
 *
**/

#include "PID.h"
#include "math.h"
/*
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 * 位置型PID，无死区，简单PID实现。
 *   参数说明
 *   __stable 是PID调节到目标状态（平稳状态），该状态有可能随时间发生变化，可以通过PID_stable来设置，平稳状态
 *   __bias 是偏差即实际反馈值与目标值的偏差，反映了差别程度
 *   __lbias 上一次的偏差历史，在求离散系统中微分有用
 *   __integral 表示积分量，即从开始到上一次的积分累积量
 *   __Kp PID的比例系数，非负
 *   __Ki PID的积分系数，非负
 *   __Kd PID的微分系数，非负

typedef struct PIDLoc{
	//私有变量意味着 只能用函数引用，用户尽量不要使用。
	float __stable;
	float __bias;
	float __lbias;
	float __integral;
	float __Kp;
	float __Ki;
	float __Kd;
	//公有变量 用户可以使用。
	float control;
	//公有方法
	float (*PID_pop)(struct PIDLoc *pid,float feedback);
	void (*PID_stable)(struct PIDLoc *pid,float stable);
	
}PIDLoc_t;
**/

/*
 * 弹出一个PID的控制量，因为PID是一个控制函数，把观测量的偏差输入，
 * 得到了控制量大小，然后输出控制量，接着在观测变化量反馈，再继续控制
**/
static float PIDLoc_pop(PIDLoc_t *pid,float feedback){
	
	//计算偏差
	pid->__bias = feedback - pid->__stable;
	//累积积分项
	pid->__integral += pid->__bias;
	//计算控制输出量
	pid->control = pid->__Kp*pid->__bias + pid->__Ki*pid->__integral + pid->__Kd*(pid->__bias-pid->__lbias);
	//更新历史偏差
	pid->__lbias = pid->__bias;
	//返回控制量
	return pid->control;
	
}

/*
 * 设置位置型PID的，理想平稳状态，也就是要到达的值
**/
static void PIDLoc_stable(PIDLoc_t *pid,float stable){
	
	//更新理想状态
	pid->__stable = stable;
	
}

/*
 *初始化位置型PID，并传入P，I，D参数，并链入pop函数。
**/
void PIDLoc_init(PIDLoc_t *pid,float Kp,float Ki,float Kd){
	
	pid->__bias = 0.0;
	pid->__integral = 0.0;
	pid->__lbias = 0.0;
	pid->__stable = 0.0;
	pid->__Kp = Kp;
	pid->__Ki = Ki;
	pid->__Kd = Kd;
	pid->control = 0.0;
	
	//链入公共方法
	pid->PID_pop = PIDLoc_pop;
	pid->PID_stable = PIDLoc_stable;
	
}

/*
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 *增量PID，简单PID实现，增量式的表达结果和最近三次的偏差有关，这样就大大提高了系统的稳定性。
 * 参数说明
 *   __stable 是PID调节到目标状态（平稳状态），该状态有可能随时间发生变化，可以通过PID_stable来设置，平稳状态
 *   __lcontrol 是上一次的控制输出量
 *   __bias 是偏差即实际反馈值与目标值的偏差，反映了差别程度
 *   __lbias 上一次的偏差历史，在求离散系统中微分有用
 *   __llbias 上上一次的偏差历史，在求离散系统中微分有用
 *   __Kp PID的比例系数，非负
 *   __Ki PID的积分系数，非负
 *   __Kd PID的微分系数，非负
 *
**
typedef struct PIDIncre{
	
	//私有变量意味着 只能用函数引用，用户尽量不要使用。
	float __stable;
	//float __lcontrol;
	float __bias;
	float __lbias;
	float __llbias;
	float __Kp;
	float __Ki;
	float __Kd;
	//公有变量 用户可以使用。
	float control;
	//公有方法
	float (*PID_pop)(struct PIDIncre *pid,float feedback);
	void (*PID_stable)(struct PIDIncre *pid,float stable);
	
}PIDIncre_t;
**/
/*
 * 增量式PID
 * 弹出一个PID的控制量，因为PID是一个控制函数，把观测量的偏差输入，
 * 得到了控制量大小，然后输出控制量，接着在观测变化量反馈，再继续控制
**/
static float PIDIncre_pop(PIDIncre_t *pid,float feedback){
	
	//计算偏差
	pid->__bias = feedback - pid->__stable;
	//通过增量方式，计算控制量输出
	pid->control += pid->__Kp*(pid->__bias-pid->__lbias) + pid->__Ki*pid->__bias + pid->__Kd*(pid->__bias-2*pid->__lbias+pid->__llbias);
	//更新历史偏差
	pid->__llbias = pid->__lbias;
	pid->__lbias = pid->__bias;
	//返回控制量
	return pid->control;
	
}

/*
 * 设置位置型PID的，理想平稳状态，也就是要到达的值
**/
static void PIDIncre_stable(PIDIncre_t *pid,float stable){
	
	//更新理想状态
	pid->__stable = stable;
	
}

/*
 *初始化增量型PID，并传入P，I，D参数，并链入pop函数。
**/
void PIDIncre_init(PIDIncre_t *pid,float Kp,float Ki,float Kd){
	
	pid->__bias = 0.0;
	pid->__lbias = 0.0;
	pid->__llbias = 0.0;
	pid->__stable = 0.0;
	pid->__Kp = Kp;
	pid->__Ki = Ki;
	pid->__Kd = Kd;
	pid->control = 0.0;
	
	//链入公共方法
	pid->PID_pop = PIDIncre_pop;
	pid->PID_stable = PIDIncre_stable;
	
}

/*
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 * 增量PID，带死区控制，即震荡大于理想状态的20%不进行正积分，只进行负积分，同时震荡小于理想状态20%只进行正积分。
 * 增加变积分，因为在PID的开始一直引入积分会增加震荡的概率，因为往往PID的开始都是和理想状态差距很大的，所以会累加大量的积分因素
 * 为了改变它，以时间为原则，Ki的数值与时间陈正比，最后到达我们预设的Ki大小并不改变。
 * 以下是程序实现
 * 参数说明
 *   __stable 是PID调节到目标状态（平稳状态），该状态有可能随时间发生变化，可以通过PID_stable来设置，平稳状态
 *   __lcontrol 是上一次的控制输出量
 *   __bias 是偏差即实际反馈值与目标值的偏差，反映了差别程度
 *   __lbias 上一次的偏差历史，在求离散系统中微分有用
 *   __llbias 上上一次的偏差历史，在求离散系统中微分有用
 *   __Kp PID的比例系数，非负
 *   __Ki PID的积分系数，非负
 *   __Kd PID的微分系数，非负
 *   __dline 表示死区控制的 上下浮动百分比，0.2即20% 1即100% 2即200%
 *   __itime 表示积分的变化时效，随时间进行成正比，随后到达预设积分系数大小，不变,这种情况特别适合刚开始偏差很小的情况，对于刚开始偏差比较大的反而会减弱静差的消除
 *   __tKi 用来存放变积分
 *
**
typedef struct PIDIncreDT{
	
	//私有变量意味着 只能用函数引用，用户尽量不要使用。
	float __stable;
	//float __lcontrol;
	float __bias;
	float __lbias;
	float __llbias;
	float __Kp;
	float __Ki;
	float __Kd;
	float __dline;
	int   __itime;
	float __tKi;
	//公有变量 用户可以使用。
	float control;
	//公有方法
	float (*PID_pop)(struct PIDIncreDT *pid,float feedback);
	void (*PID_stable)(struct PIDIncreDT *pid,float stable);
	
}PIDIncreDT_t;
**/
/*
 * 带死区和时间权值的增量式PID
 * 弹出一个PID的控制量，因为PID是一个控制函数，把观测量的偏差输入，
 * 得到了控制量大小，然后输出控制量，接着在观测变化量反馈，再继续控制
**/
static float PIDIncreDT_pop(PIDIncreDT_t *pid,float feedback){
	
	//计算偏差
	pid->__bias = feedback - pid->__stable;
	//判断偏差是否在死区控制区
	if(fabs(pid->__bias) > fabs(pid->__stable)*pid->__dline){
		//在死区外，不累积积分项
		pid->control += pid->__Kp*(pid->__bias-pid->__lbias) + pid->__Kd*(pid->__bias-2*pid->__lbias+pid->__llbias);
	
	}else{
		//在死区内，开始计算时间权值，知道等于预设的Ki
		if(pid->__tKi < pid->__Ki){
			pid->__tKi += pid->__Ki/(float)pid->__itime;
		}
		//在死区内，累积积分项，积分系数为__tKi
		pid->control += pid->__Kp*(pid->__bias-pid->__lbias) + pid->__tKi*pid->__bias + pid->__Kd*(pid->__bias-2*pid->__lbias+pid->__llbias);
	
	}
	//更新历史偏差
	pid->__llbias = pid->__lbias;
	pid->__lbias = pid->__bias;
	//返回控制量
	return pid->control;
	
}

/*
 * 设置带死区和时间权值的增量式PID，理想平稳状态，也就是要到达的值
**/
static void PIDIncreDT_stable(PIDIncreDT_t *pid,float stable){
	
	//更新理想状态
	pid->__stable = stable;
	
}

/*
 *初始化带死区和时间权值的增量式PID，并传入P，I，D参数，并链入pop函数。
**/
void PIDIncreDT_init(PIDIncreDT_t *pid,float Kp,float Ki,float Kd,float dline,float itime){
	
	pid->__bias = 0.0;
	pid->__lbias = 0.0;
	pid->__llbias = 0.0;
	pid->__stable = 0.0;
	pid->__Kp = Kp;
	pid->__Ki = Ki;
	pid->__tKi = 0.0;
	pid->__Kd = Kd;
	pid->control = 0.0;
	
	//设置死区和时间影响
	pid->__dline = dline;
	pid->__itime = itime;
	
	//链入公共方法
	pid->PID_pop = PIDIncreDT_pop;
	pid->PID_stable = PIDIncreDT_stable;
	
}

/*
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 * 增量PID，带死区控制，即震荡大于理想状态的20%不进行正积分，只进行负积分，同时震荡小于理想状态20%只进行正积分。
 * 增加变积分，因为在PID的开始一直引入积分会增加震荡的概率，因为往往PID的开始都是和理想状态差距很大的，所以会累加大量的积分因素
 * 为了改变它，通过对比实际值与理想状态的差距百分比，来决定积分量，当然越接近积分越接近预设定值。消除静差越有效。这种情况特别适合刚开始偏差很大的情况，对于刚开始偏差比较小的反而会减弱静差的消除
 * 随后加入了时间作为权值，变积分大小与时间成反比，极大了减小了累积偏差。
 *
 * 说简单点，就是在上面的基础上，修改了变积分的规则，不仅仅是加入了时间权值，还加入了偏差权值，时间越长变积分越大，偏差越小变积分越大。
 *
 * 参数说明
 *   __stable 是PID调节到目标状态（平稳状态），该状态有可能随时间发生变化，可以通过PID_stable来设置，平稳状态
 *   __lcontrol 是上一次的控制输出量
 *   __bias 是偏差即实际反馈值与目标值的偏差，反映了差别程度
 *   __lbias 上一次的偏差历史，在求离散系统中微分有用
 *   __llbias 上上一次的偏差历史，在求离散系统中微分有用
 *   __Kp PID的比例系数，非负
 *   __Ki PID的积分系数，非负
 *   __Kd PID的微分系数，非负
 *   __dline 表示死区控制的 上下浮动百分比，0.2即20% 1即100% 2即200%
 *   __itime 表示积分的变化时效，随时间进行成正比，随后到达预设积分系数大小，不变,这种情况特别适合刚开始偏差很小的情况，对于刚开始偏差比较大的反而会减弱静差的消除
 *   __tKi 用来存放变积分
 *
**
typedef struct PIDIncreDTB{
	
	//私有变量意味着 只能用函数引用，用户尽量不要使用。
	float __stable;
	//float __lcontrol;
	float __bias;
	float __lbias;
	float __llbias;
	float __Kp;
	float __Ki;
	float __Kd;
	float __dline;
	int   __itime;
	float __tKi;
	//公有变量 用户可以使用。
	float control;
	//公有方法
	float (*PID_pop)(struct PIDIncreDTB *pid,float feedback);
	void (*PID_stable)(struct PIDIncreDTB *pid,float stable);
	
}PIDIncreDTB_t;
**/

/*
 * 带死区和时间权值的增量式PID
 * 弹出一个PID的控制量，因为PID是一个控制函数，把观测量的偏差输入，
 * 得到了控制量大小，然后输出控制量，接着在观测变化量反馈，再继续控制
**/
static float PIDIncreDTB_pop(PIDIncreDTB_t *pid,float feedback){
	
	//计算偏差
	pid->__bias = feedback - pid->__stable;
	//判断偏差是否在死区控制区
	if(fabs(pid->__bias) > fabs(pid->__stable)*pid->__dline){
		//在死区外，不累积积分项
		pid->control += pid->__Kp*(pid->__bias-pid->__lbias) + pid->__Kd*(pid->__bias-(float)2.0*pid->__lbias+pid->__llbias);
	
	}else{
		//在死区内，开始计算时间权值与偏差权值，知道等于预设的Ki 
		pid->__tKi = pid->__Ki*(1-fabs(pid->__bias)/fabs(pid->__stable*pid->__dline))*((float)1.0/(float)pid->__itime);
		//更新__itime
		if(pid->__itime > 1){
			pid->__itime--;
		}
		//在死区内，累积积分项，积分系数为__tKi
		pid->control += pid->__Kp*(pid->__bias-pid->__lbias) + pid->__tKi*pid->__bias + pid->__Kd*(pid->__bias-2*pid->__lbias+pid->__llbias);
	
	}
	//更新历史偏差
	pid->__llbias = pid->__lbias;
	pid->__lbias = pid->__bias;
	//返回控制量
	return pid->control;
	
}

/*
 * 设置带死区和时间权值的增量式PID，理想平稳状态，也就是要到达的值
**/
static void PIDIncreDTB_stable(PIDIncreDTB_t *pid,float stable){
	
	//更新理想状态
	pid->__stable = stable;
	
}

/*
 *初始化带死区和时间权值的增量式PID，并传入P，I，D参数，并链入pop函数。
**/
void PIDIncreDTB_init(PIDIncreDTB_t *pid,float Kp,float Ki,float Kd,float dline,float itime){
	
	pid->__bias = 0.0;
	pid->__lbias = 0.0;
	pid->__llbias = 0.0;
	pid->__stable = 0.0;
	pid->__Kp = Kp;
	pid->__Ki = Ki;
	pid->__tKi = 0.0;
	pid->__Kd = Kd;
	pid->control = 0.0;
	
	//设置死区和时间影响
	pid->__dline = dline;
	pid->__itime = itime;
	
	//链入公共方法
	pid->PID_pop = PIDIncreDTB_pop;
	pid->PID_stable = PIDIncreDTB_stable;
	
}
