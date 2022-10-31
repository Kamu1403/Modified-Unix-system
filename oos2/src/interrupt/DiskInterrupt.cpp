#include "DiskInterrupt.h"
#include "Kernel.h"
#include "Regs.h"
#include "ATADriver.h"
#include "IOPort.h"
#include "Chip8259A.h"

void DiskInterrupt::DiskInterruptEntrance()
{
	SaveContext();			/* �����ж��ֳ� */

	SwitchToKernel();		/* �������̬ */

	CallHandler(ATADriver, ATAHandler);		/* ���ô����жϴ������ */

	/* ��������8259A�жϿ���оƬ�ֱ���EOI��� */
	IOPort::OutByte(Chip8259A::MASTER_IO_PORT_1, Chip8259A::EOI);
	IOPort::OutByte(Chip8259A::SLAVE_IO_PORT_1, Chip8259A::EOI);

	/* ��ȡ���ж���ָ��(��Ӳ��ʵʩ)ѹ�����ջ��pt_context��
	* �����Ϳ��Է���context.xcs�е�OLD_CPL���ж���ǰ̬
	* ���û�̬���Ǻ���̬��
	*/
	struct pt_context *context;
	__asm__ __volatile__ ("	movl %%ebp, %0; addl $0x4, %0 " : "+m" (context) );

	if( context->xcs & USER_MODE ) /*��ǰΪ�û�̬*/
	{
		while(true)
		{
			X86Assembly::CLI();	/* ��������ȼ���Ϊ7�� */
			
			if(Kernel::Instance().GetProcessManager().RunRun > 0)
			{
				X86Assembly::STI();	/* ��������ȼ���Ϊ0�� */
				Kernel::Instance().GetProcessManager().Swtch();
			}
			else
			{
				break;	/* ���runrun == 0������ջ�ص��û�̬�����û������ִ�� */
			}
		}
	}
	
	RestoreContext();		/* �ָ��ֳ� */

	Leave();				/* �ֹ�����ջ֡ */

	InterruptReturn();		/* �˳��ж� */
}
