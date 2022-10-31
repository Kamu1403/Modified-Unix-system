#ifndef TIME_INTERRUPT_H
#define TIME_INTERRUPT_H

class Time
{
	/* 
	 * ע������Bochs��ʱ���жϲ�׼ȷ��һ����ʱ���жϴ���Զ����60�Σ�
	 * Ϊ�˾����ܼ�Сϵͳʱ���������SCHMAG��HZ��ֵͬ������
	 * ����ȡֵ�������Է�����Bochs�±Ƚ����룬�����˽�Ӱ�췶Χ���ٽ��иĶ�������
	 */
public:
	static const int SCHMAG = 10 * 2;	/* ÿ���Ӽ��ٵ�p_cpuħ�� */

	static const int HZ = 60 * 2;		/* ÿ����ʱ���жϴ��� */

	static int lbolt;				/* �ۼƽ��յ���ʱ���жϴ��� */
	
	static unsigned int time;		/* ϵͳȫ��ʱ�䣬��1970��1��1����������� */

	static unsigned int tout;		/* ����ʱ˯�߽�����Ӧ�����ѵ�ʱ������Сֵ */

	/* ʱ���ж���ں��������ַ�����IDT�Ĵ����ж϶�Ӧ�ж����� */
	static void TimeInterruptEntrance();

	/* ʱ���жϴ�������ά��ϵͳʱ���������������ռ��CPUʱ�䣬����˯�߽��̺���ʱ���еȹ��� */
	static void Clock(struct pt_regs* regs, struct pt_context* context);
};

#endif
