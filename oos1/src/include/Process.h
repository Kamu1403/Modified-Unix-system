#ifndef PROCESS_H
#define PROCESS_H

#include "Text.h"
#include "TTy.h"
#include "Regs.h"

/*
 * Process����UNIX V6�н��̿��ƿ�proc�ṹ��Ӧ������ֻ�ı�
 * ���������޸ĳ�Ա�ṹ���֣��Լ���UNIX V6��proc�ṹ�г�Ա
 * ʹ�õ��������ͽ����ʵ����ģ��Է����ִ�����Ĵ�����
 */
class Process
{
public:
	enum ProcessState	/* ����״̬ */
	{
		SNULL	= 0,	/* δ��ʼ����״̬ */
		SSLEEP	= 1,	/* ������Ȩ˯�� */
		SWAIT	= 2,	/* ������Ȩ˯�� */
		SRUN	= 3,	/* ���С�����״̬ */
		SIDL	= 4,	/* ���̴���ʱ���м�״̬ */
		SZOMB	= 5,	/* ������ֹʱ���м�״̬ */
		SSTOP	= 6		/* ������������ */
	};

	enum ProcessFlag	/* ���̱�־λ */
	{
		SLOAD	= 0x1,	/* ����ͼ�����ڴ��� */
		SSYS	= 0x2,	/* ϵͳ����ͼ�񣬲��������� */
		SLOCK	= 0x4,	/* ���иñ�־�Ľ���ͼ���ݲ������� */
		SSWAP	= 0x8,	/* �ý��̱�����ʱͼ����ڽ������� */
		STRC	= 0x10,	/* ���ӽ��̸��ٱ�־��UNIX V6++δ��Чʹ�õ� */
		STWED	= 0x20	/* ���ӽ��̸��ٱ�־��UNIX V6++δ��Чʹ�õ� */
	};
public:
	Process();
	~Process();
	void SetRun();								/* ���ѵ�ǰ���̣�ת�����״̬ */
	
	void SetPri();								/* ����ռ��CPUʱ����㵱ǰ���������� */
	
	bool IsSleepOn(unsigned long chan);			/* ��鵱ǰ����˯��ԭ���Ƿ�Ϊchan */
	
	void Sleep(unsigned long chan, int pri);	/* ʹ��ǰ����ת��˯��״̬ */
	
	void Expand(unsigned int newSize);			/* �ı����ռ�õ��ڴ��С */
	
	void Exit();								/* Exit()ϵͳ���ô������ */
	
	void Clone(Process& proc);					/* ��p_pid֮�⣬�ӽ��̿���������Process�ṹ */
	
	void SStack();                              /* ��ջ���ʱ���Զ���չ��ջ */

	void SBreak();								/* brk()ϵͳ���ô������ */
	
	void PSignal(int signal);					/* ��ǰ���̷����źš�signal��Ҫ���͵��ź��� */

	void PSig(struct pt_context* pContext);		/* �Ե�ǰ���̽��յ����źŽ��д��� */

	void Nice();								/* �û����ü��������������ƫ��ֵ */

	void Ssig();								/* �����û��Զ��źŴ���ʽ��ϵͳ���ô����� */

	int IsSig();								/* �жϵ�ǰ�����Ƿ���յ��źš�
												 * 
												 * ������ÿ���жϡ��쳣��ϵͳ����ĩβ�˳�����̬ʱ���Լ�������Ȩ
												 * ˯����β���ܻ���p_sig��p_sig�н��յ��źű���Խ��̲���Ӱ�죬
												 * ������Ӧ�ź�ʱ���̻�Ըı�ԭ��ִ�����̡�
												 * 
												 * ����ֵ���ź��������ź��򷵻�0��
												 */

public:
	/* ���ڱ�ʶ���̵ı�ʶ */
	short p_uid;		/* �û�ID */
	int p_pid;			/* ���̱�ʶ�������̱�� */
	int p_ppid;			/* �����̱�ʶ�� */

	/* �����ڴ���ͼ����Ϣλ�� */
	unsigned long	p_addr; /* TBD user�ṹ��ppda���������ڴ��еĵ�ַ���������ҳ���е�ĳһ�� */
	unsigned int	p_size; /* ���������Ķεĳ��ȣ����ֽڵ�λ */
	Text*	p_textp;		/* ָ��ý��������еĴ���ε������� */

	/* ���̵���״̬ */
	ProcessState	p_stat;	/* ���̵�ǰ״̬ */
	int p_flag;			/* ���̱�־λ�����Խ����״̬��� */

	int p_pri;			/* ���������� */
	int p_cpu;			/* cpuֵ�����ڼ���p_pri */
	int p_nice;			/* ����������΢������ */
	int p_time;			/* ����������(�ڴ���)פ��ʱ�� */

	unsigned long	p_wchan;	/* ����˯��ԭ��һ��Ϊ�ڴ��ַ���ȴ�ĳ���ں����� */
	
	/* �ź������̨�ն� */
	int p_sig;			/* �����ź� */
	TTy* p_ttyp;		/* ����tty�ṹ��ַ */
	unsigned long p_sigmap;
};

#endif
