#ifndef IOPORT_H
#define IOPORT_H


/*
	����ʱ�䣺3:09 PM 2008-8-9
	˵���������˶�IN,OUTָ��ĳ���
		ʹ����C++ Inline Assembly ��װ��IN,OUTָ�
		���ڶ�CHIPS�Լ�����reprogramʱ��˿ڵĶ�д�����ݴ��͡�
*/

/*
	��2�ֿɹ�ѡ���ʵ�ַ����� 
	(1) ʹ��class IOPort��IN,OUTָ����з�װ��IN��OUTʵ��Ϊstatic��Ա������
	(2) ʹ��namespace IOPort��IN,OUTָ����з�װ��
	
	�������ַ������������ڣ�  ����ʱ�﷨��ͬ��
	����һ��  class IOPort ��ʵ�ַ�ʽ
	����ʱ��  IOPort::IN8(unsigned short port);        
	
	��������  namespace IOPort��ʵ�ַ�ʽ
	����ʱ��  ��û������using namespace IOPort; ����£� IOPort::IN8(unsigned short port);
			������using namespace IOPort;֮�� ֱ��IN8(unsigned short port);   
			
	ע��	���ַ�����ʵ�ִ������涼�У��������ַ���ʵ�ִ������class��namespace�ؼ��ֵ�����
		namespaceʵ�ַ�������ǰû��static�ؼ���֮�⣬û��������
		Ŀǰenable�˷���һ�� �������Ĵ��뱻ע�͵��ˡ�
*/

/*
	����C++ Inline Assembly���ο���AT&T ��� (AT&T ASM) �ο� �� 
	@ http://blog.csdn.net/robin_qiu/archive/2006/03/02/613904.aspx
	���������������ҳ�Ҵ򲻿��ˣ�������ҳ�Ҵ������ˡ�
*/

/*����һ��*/
class IOPort
{	
	//���ں�������Inline Assembly��ԭ�򣬺�������д��.cpp�ļ��к���������⡣
	//���Ըɴ�Ѻ�������������һ��д��class IOPort�������������ˡ�
	
	public:
		//��ָ���˿�port����8�������ݣ�����data����Ϊ����ֵ
		static inline unsigned char InByte(unsigned short port)
		{
			unsigned char data;
			__asm__ __volatile__("inb %%dx, %%al"
						:"=a" (data)
						:"d" (port) );
			return data;
		}
		
		//��ָ���˿�port����16�������ݣ�����data����Ϊ����ֵ
		static inline unsigned short InWord(unsigned short port)
		{
			unsigned short data;
			__asm__ __volatile__("inw %%dx, %%ax"
						:"=a" (data)
						:"d"	(port)	);
			return data;
		}
		
		//��ָ���˿�port����32�������ݣ�����data����Ϊ����ֵ
		static inline unsigned int InDWord(unsigned short port)
		{
			unsigned int data;
			__asm__ __volatile__("inl %%dx, %%eax"
						:"=a" (data)
						:"d" (port)	);
			return data;
		}
	//=====================================================================
	//�����Ƕ�IN�ķ�װ�������Ƕ�OUTָ��ķ�װ
		
		//��8��������data��д�뵽ָ���˿�port��
		static inline void OutByte(unsigned short port, unsigned char data)
		{
			__asm__ __volatile__("outb %%al, %%dx"
						:: "d" (port), "a"(data)	);
		}
		
		//��16��������data��д�뵽ָ���˿�port��
		static inline void OutWord(unsigned short port, unsigned short data)
		{
			__asm__ __volatile__("outw %%ax, %%dx"
						:: "d" (port), "a"(data)	);
		}
		
		//��32��������data��д�뵽ָ���˿�port��
		static inline void OutDWord(unsigned short port, unsigned int data)
		{
			__asm__ __volatile__("outl %%eax, %%dx"
						:: "d" (port), "a"(data)	);
		}
		
}; // end of class IOPort declearation

/*end of ����һ */ 

/*������:
namespace IOPort{
		
		//��ָ���˿�port����8�������ݣ�����data����Ϊ����ֵ
		inline unsigned char in8(unsigned short port)
		{
			unsigned char data;
			__asm__ __volatile__("inb %%dx, %%al"
						:"=a" (data)
						:"d" (port) );
			return data;
		}
		
		//��ָ���˿�port����16�������ݣ�����data����Ϊ����ֵ
		inline unsigned short in16(unsigned short port)
		{
			unsigned short data;
			__asm__ __volatile__("inw %%dx, %%ax"
						:"=a" (data)
						:"d"	(port)	);
			return data;
		}
		
		//��ָ���˿�port����32�������ݣ�����data����Ϊ����ֵ
		inline unsigned int in32(unsigned short port)
		{
			unsigned int data;
			__asm__ __volatile__("inl %%dx, %%eax"
						:"=a" (data)
						:"d" (port)	);
			return data;
		}
	//=====================================================================
	//�����Ƕ�IN�ķ�װ�������Ƕ�OUTָ��ķ�װ
		
		//��8��������data��д�뵽ָ���˿�port��
		inline void out8(unsigned short port, unsigned char data)
		{
			__asm__ __volatile__("outb %%al, %%dx" 
						:: "d" (port), "a"(data)	);
		}
		
		//��16��������data��д�뵽ָ���˿�port��
		inline void out16(unsigned short port, unsigned short data)
		{
			__asm__ __volatile__("outw %%ax, %%dx"
						:: "d" (port), "a"(data)	);
		}
		
		//��32��������data��д�뵽ָ���˿�port��
		inline void out32(unsigned short port, unsigned int data)
		{
			__asm__ __volatile__("outl %%eax, %%dx"
						:: "d" (port), "a"(data)	);
		}
}//end of IOPort namespace

end of ������ */ 

#endif
