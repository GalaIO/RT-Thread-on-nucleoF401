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
 *				���������������һЩPython�Ĺ淶����__varһ�������������»��߿�ͷʱ����ζ�����ǽṹ���е�
 *				˽�б�������var��Ѱ���÷���ʾ����������ֵ��˵�����Ǹù���Ȼּ��struct�����Ľṹ������Ч��
 *
**/

#include "PID.h"
#include "math.h"
/*
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 * λ����PID������������PIDʵ�֡�
 *   ����˵��
 *   __stable ��PID���ڵ�Ŀ��״̬��ƽ��״̬������״̬�п�����ʱ�䷢���仯������ͨ��PID_stable�����ã�ƽ��״̬
 *   __bias ��ƫ�ʵ�ʷ���ֵ��Ŀ��ֵ��ƫ���ӳ�˲��̶�
 *   __lbias ��һ�ε�ƫ����ʷ��������ɢϵͳ��΢������
 *   __integral ��ʾ�����������ӿ�ʼ����һ�εĻ����ۻ���
 *   __Kp PID�ı���ϵ�����Ǹ�
 *   __Ki PID�Ļ���ϵ�����Ǹ�
 *   __Kd PID��΢��ϵ�����Ǹ�

typedef struct PIDLoc{
	//˽�б�����ζ�� ֻ���ú������ã��û�������Ҫʹ�á�
	float __stable;
	float __bias;
	float __lbias;
	float __integral;
	float __Kp;
	float __Ki;
	float __Kd;
	//���б��� �û�����ʹ�á�
	float control;
	//���з���
	float (*PID_pop)(struct PIDLoc *pid,float feedback);
	void (*PID_stable)(struct PIDLoc *pid,float stable);
	
}PIDLoc_t;
**/

/*
 * ����һ��PID�Ŀ���������ΪPID��һ�����ƺ������ѹ۲�����ƫ�����룬
 * �õ��˿�������С��Ȼ������������������ڹ۲�仯���������ټ�������
**/
static float PIDLoc_pop(PIDLoc_t *pid,float feedback){
	
	//����ƫ��
	pid->__bias = feedback - pid->__stable;
	//�ۻ�������
	pid->__integral += pid->__bias;
	//������������
	pid->control = pid->__Kp*pid->__bias + pid->__Ki*pid->__integral + pid->__Kd*(pid->__bias-pid->__lbias);
	//������ʷƫ��
	pid->__lbias = pid->__bias;
	//���ؿ�����
	return pid->control;
	
}

/*
 * ����λ����PID�ģ�����ƽ��״̬��Ҳ����Ҫ�����ֵ
**/
static void PIDLoc_stable(PIDLoc_t *pid,float stable){
	
	//��������״̬
	pid->__stable = stable;
	
}

/*
 *��ʼ��λ����PID��������P��I��D������������pop������
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
	
	//���빫������
	pid->PID_pop = PIDLoc_pop;
	pid->PID_stable = PIDLoc_stable;
	
}

/*
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 *����PID����PIDʵ�֣�����ʽ�ı������������ε�ƫ���йأ������ʹ�������ϵͳ���ȶ��ԡ�
 * ����˵��
 *   __stable ��PID���ڵ�Ŀ��״̬��ƽ��״̬������״̬�п�����ʱ�䷢���仯������ͨ��PID_stable�����ã�ƽ��״̬
 *   __lcontrol ����һ�εĿ��������
 *   __bias ��ƫ�ʵ�ʷ���ֵ��Ŀ��ֵ��ƫ���ӳ�˲��̶�
 *   __lbias ��һ�ε�ƫ����ʷ��������ɢϵͳ��΢������
 *   __llbias ����һ�ε�ƫ����ʷ��������ɢϵͳ��΢������
 *   __Kp PID�ı���ϵ�����Ǹ�
 *   __Ki PID�Ļ���ϵ�����Ǹ�
 *   __Kd PID��΢��ϵ�����Ǹ�
 *
**
typedef struct PIDIncre{
	
	//˽�б�����ζ�� ֻ���ú������ã��û�������Ҫʹ�á�
	float __stable;
	//float __lcontrol;
	float __bias;
	float __lbias;
	float __llbias;
	float __Kp;
	float __Ki;
	float __Kd;
	//���б��� �û�����ʹ�á�
	float control;
	//���з���
	float (*PID_pop)(struct PIDIncre *pid,float feedback);
	void (*PID_stable)(struct PIDIncre *pid,float stable);
	
}PIDIncre_t;
**/
/*
 * ����ʽPID
 * ����һ��PID�Ŀ���������ΪPID��һ�����ƺ������ѹ۲�����ƫ�����룬
 * �õ��˿�������С��Ȼ������������������ڹ۲�仯���������ټ�������
**/
static float PIDIncre_pop(PIDIncre_t *pid,float feedback){
	
	//����ƫ��
	pid->__bias = feedback - pid->__stable;
	//ͨ��������ʽ��������������
	pid->control += pid->__Kp*(pid->__bias-pid->__lbias) + pid->__Ki*pid->__bias + pid->__Kd*(pid->__bias-2*pid->__lbias+pid->__llbias);
	//������ʷƫ��
	pid->__llbias = pid->__lbias;
	pid->__lbias = pid->__bias;
	//���ؿ�����
	return pid->control;
	
}

/*
 * ����λ����PID�ģ�����ƽ��״̬��Ҳ����Ҫ�����ֵ
**/
static void PIDIncre_stable(PIDIncre_t *pid,float stable){
	
	//��������״̬
	pid->__stable = stable;
	
}

/*
 *��ʼ��������PID��������P��I��D������������pop������
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
	
	//���빫������
	pid->PID_pop = PIDIncre_pop;
	pid->PID_stable = PIDIncre_stable;
	
}

/*
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 * ����PID�����������ƣ����𵴴�������״̬��20%�����������֣�ֻ���и����֣�ͬʱ��С������״̬20%ֻ���������֡�
 * ���ӱ���֣���Ϊ��PID�Ŀ�ʼһֱ������ֻ������𵴵ĸ��ʣ���Ϊ����PID�Ŀ�ʼ���Ǻ�����״̬���ܴ�ģ����Ի��ۼӴ����Ļ�������
 * Ϊ�˸ı�������ʱ��Ϊԭ��Ki����ֵ��ʱ������ȣ���󵽴�����Ԥ���Ki��С�����ı䡣
 * �����ǳ���ʵ��
 * ����˵��
 *   __stable ��PID���ڵ�Ŀ��״̬��ƽ��״̬������״̬�п�����ʱ�䷢���仯������ͨ��PID_stable�����ã�ƽ��״̬
 *   __lcontrol ����һ�εĿ��������
 *   __bias ��ƫ�ʵ�ʷ���ֵ��Ŀ��ֵ��ƫ���ӳ�˲��̶�
 *   __lbias ��һ�ε�ƫ����ʷ��������ɢϵͳ��΢������
 *   __llbias ����һ�ε�ƫ����ʷ��������ɢϵͳ��΢������
 *   __Kp PID�ı���ϵ�����Ǹ�
 *   __Ki PID�Ļ���ϵ�����Ǹ�
 *   __Kd PID��΢��ϵ�����Ǹ�
 *   __dline ��ʾ�������Ƶ� ���¸����ٷֱȣ�0.2��20% 1��100% 2��200%
 *   __itime ��ʾ���ֵı仯ʱЧ����ʱ����г����ȣ���󵽴�Ԥ�����ϵ����С������,��������ر��ʺϸտ�ʼƫ���С����������ڸտ�ʼƫ��Ƚϴ�ķ�����������������
 *   __tKi ������ű����
 *
**
typedef struct PIDIncreDT{
	
	//˽�б�����ζ�� ֻ���ú������ã��û�������Ҫʹ�á�
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
	//���б��� �û�����ʹ�á�
	float control;
	//���з���
	float (*PID_pop)(struct PIDIncreDT *pid,float feedback);
	void (*PID_stable)(struct PIDIncreDT *pid,float stable);
	
}PIDIncreDT_t;
**/
/*
 * ��������ʱ��Ȩֵ������ʽPID
 * ����һ��PID�Ŀ���������ΪPID��һ�����ƺ������ѹ۲�����ƫ�����룬
 * �õ��˿�������С��Ȼ������������������ڹ۲�仯���������ټ�������
**/
static float PIDIncreDT_pop(PIDIncreDT_t *pid,float feedback){
	
	//����ƫ��
	pid->__bias = feedback - pid->__stable;
	//�ж�ƫ���Ƿ�������������
	if(fabs(pid->__bias) > fabs(pid->__stable)*pid->__dline){
		//�������⣬���ۻ�������
		pid->control += pid->__Kp*(pid->__bias-pid->__lbias) + pid->__Kd*(pid->__bias-2*pid->__lbias+pid->__llbias);
	
	}else{
		//�������ڣ���ʼ����ʱ��Ȩֵ��֪������Ԥ���Ki
		if(pid->__tKi < pid->__Ki){
			pid->__tKi += pid->__Ki/(float)pid->__itime;
		}
		//�������ڣ��ۻ����������ϵ��Ϊ__tKi
		pid->control += pid->__Kp*(pid->__bias-pid->__lbias) + pid->__tKi*pid->__bias + pid->__Kd*(pid->__bias-2*pid->__lbias+pid->__llbias);
	
	}
	//������ʷƫ��
	pid->__llbias = pid->__lbias;
	pid->__lbias = pid->__bias;
	//���ؿ�����
	return pid->control;
	
}

/*
 * ���ô�������ʱ��Ȩֵ������ʽPID������ƽ��״̬��Ҳ����Ҫ�����ֵ
**/
static void PIDIncreDT_stable(PIDIncreDT_t *pid,float stable){
	
	//��������״̬
	pid->__stable = stable;
	
}

/*
 *��ʼ����������ʱ��Ȩֵ������ʽPID��������P��I��D������������pop������
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
	
	//����������ʱ��Ӱ��
	pid->__dline = dline;
	pid->__itime = itime;
	
	//���빫������
	pid->PID_pop = PIDIncreDT_pop;
	pid->PID_stable = PIDIncreDT_stable;
	
}

/*
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------------------------------------------
 * ����PID�����������ƣ����𵴴�������״̬��20%�����������֣�ֻ���и����֣�ͬʱ��С������״̬20%ֻ���������֡�
 * ���ӱ���֣���Ϊ��PID�Ŀ�ʼһֱ������ֻ������𵴵ĸ��ʣ���Ϊ����PID�Ŀ�ʼ���Ǻ�����״̬���ܴ�ģ����Ի��ۼӴ����Ļ�������
 * Ϊ�˸ı�����ͨ���Ա�ʵ��ֵ������״̬�Ĳ��ٷֱȣ�����������������ȻԽ�ӽ�����Խ�ӽ�Ԥ�趨ֵ����������Խ��Ч����������ر��ʺϸտ�ʼƫ��ܴ����������ڸտ�ʼƫ��Ƚ�С�ķ�����������������
 * ��������ʱ����ΪȨֵ������ִ�С��ʱ��ɷ��ȣ������˼�С���ۻ�ƫ�
 *
 * ˵�򵥵㣬����������Ļ����ϣ��޸��˱���ֵĹ��򣬲������Ǽ�����ʱ��Ȩֵ����������ƫ��Ȩֵ��ʱ��Խ�������Խ��ƫ��ԽС�����Խ��
 *
 * ����˵��
 *   __stable ��PID���ڵ�Ŀ��״̬��ƽ��״̬������״̬�п�����ʱ�䷢���仯������ͨ��PID_stable�����ã�ƽ��״̬
 *   __lcontrol ����һ�εĿ��������
 *   __bias ��ƫ�ʵ�ʷ���ֵ��Ŀ��ֵ��ƫ���ӳ�˲��̶�
 *   __lbias ��һ�ε�ƫ����ʷ��������ɢϵͳ��΢������
 *   __llbias ����һ�ε�ƫ����ʷ��������ɢϵͳ��΢������
 *   __Kp PID�ı���ϵ�����Ǹ�
 *   __Ki PID�Ļ���ϵ�����Ǹ�
 *   __Kd PID��΢��ϵ�����Ǹ�
 *   __dline ��ʾ�������Ƶ� ���¸����ٷֱȣ�0.2��20% 1��100% 2��200%
 *   __itime ��ʾ���ֵı仯ʱЧ����ʱ����г����ȣ���󵽴�Ԥ�����ϵ����С������,��������ر��ʺϸտ�ʼƫ���С����������ڸտ�ʼƫ��Ƚϴ�ķ�����������������
 *   __tKi ������ű����
 *
**
typedef struct PIDIncreDTB{
	
	//˽�б�����ζ�� ֻ���ú������ã��û�������Ҫʹ�á�
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
	//���б��� �û�����ʹ�á�
	float control;
	//���з���
	float (*PID_pop)(struct PIDIncreDTB *pid,float feedback);
	void (*PID_stable)(struct PIDIncreDTB *pid,float stable);
	
}PIDIncreDTB_t;
**/

/*
 * ��������ʱ��Ȩֵ������ʽPID
 * ����һ��PID�Ŀ���������ΪPID��һ�����ƺ������ѹ۲�����ƫ�����룬
 * �õ��˿�������С��Ȼ������������������ڹ۲�仯���������ټ�������
**/
static float PIDIncreDTB_pop(PIDIncreDTB_t *pid,float feedback){
	
	//����ƫ��
	pid->__bias = feedback - pid->__stable;
	//�ж�ƫ���Ƿ�������������
	if(fabs(pid->__bias) > fabs(pid->__stable)*pid->__dline){
		//�������⣬���ۻ�������
		pid->control += pid->__Kp*(pid->__bias-pid->__lbias) + pid->__Kd*(pid->__bias-(float)2.0*pid->__lbias+pid->__llbias);
	
	}else{
		//�������ڣ���ʼ����ʱ��Ȩֵ��ƫ��Ȩֵ��֪������Ԥ���Ki 
		pid->__tKi = pid->__Ki*(1-fabs(pid->__bias)/fabs(pid->__stable*pid->__dline))*((float)1.0/(float)pid->__itime);
		//����__itime
		if(pid->__itime > 1){
			pid->__itime--;
		}
		//�������ڣ��ۻ����������ϵ��Ϊ__tKi
		pid->control += pid->__Kp*(pid->__bias-pid->__lbias) + pid->__tKi*pid->__bias + pid->__Kd*(pid->__bias-2*pid->__lbias+pid->__llbias);
	
	}
	//������ʷƫ��
	pid->__llbias = pid->__lbias;
	pid->__lbias = pid->__bias;
	//���ؿ�����
	return pid->control;
	
}

/*
 * ���ô�������ʱ��Ȩֵ������ʽPID������ƽ��״̬��Ҳ����Ҫ�����ֵ
**/
static void PIDIncreDTB_stable(PIDIncreDTB_t *pid,float stable){
	
	//��������״̬
	pid->__stable = stable;
	
}

/*
 *��ʼ����������ʱ��Ȩֵ������ʽPID��������P��I��D������������pop������
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
	
	//����������ʱ��Ӱ��
	pid->__dline = dline;
	pid->__itime = itime;
	
	//���빫������
	pid->PID_pop = PIDIncreDTB_pop;
	pid->PID_stable = PIDIncreDTB_stable;
	
}
