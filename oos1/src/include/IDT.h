#ifndef	IDT_H
#define	IDT_H

/* 
 * ������IDT(�ж���������)��ص��ࡣ
 * ��Ҫ��������3��class: 
 * 	1. struct GateDescriptor
 *	2. struct IDTR
 * 	3. class IDT
 */

/* ������IDT��ÿһ�����������ĸ�ʽ */
struct GateDescriptor
{
	unsigned short	m_Low16BitsOffset;			/*OFFSET�ĵ�16λ*/
	unsigned short	m_SegmentSelector;			/*��ѡ����*/
	unsigned char	m_Reserved : 5;				/*�������򣬳�5��bit*/
	unsigned char	m_Zero : 3;					/*ȫ������*/
	unsigned char	m_Type : 4;					/*����������.  0xEΪ�ж���  0xFΪ������*/
	unsigned char	m_System : 1;				/*1��ϵͳ������  0�����롢���ݶ�������*/
	unsigned char	m_DPL : 2;					/*�������������ȼ�*/
	unsigned char	m_SegmentPresent : 1;		/*���ڱ�־λ*/
	unsigned short	m_High16BitsOffset;			/*OFFSET�ĸ�16λ*/
}__attribute__((packed));
/* һ��˵����
 * 	__attribute__((packed))��Ϊ�˷�ֹ��ĳ�Ա�����������ֽڶ�����Ҫ�����������϶��
 * 	�ᵼ��sizeof(GateDescriptor)����8 Byte�� ��������Ļ���CPU����[GDTR+ �жϺ�*8]��
 * 	��ȡIDT�е�Descriptor�ͻ��ֽڴ�λ��(�������ﲻ��packed����, ȫ������λ֮��Ӧ��Ҳ��8byte)
 */


/* IDT�����ַ���޳��ṹ�嶨�壬���ڼ��ص�IDTR�Ĵ��� */
struct IDTR
{
	unsigned short	m_Limit;		/* IDT���޳� */
	unsigned int	m_BaseAddress;	/* IDT����ʼ��ַ(���Ե�ַ) */
}__attribute__((packed));


/* IDT�ж���������Ķ��壬��256��GateDescriptor��ɣ��ܳ���Ϊ2048�ֽ� */
class IDT
{
	/* static member functions */
public:
	static void DefaultInterruptHandler();		/* Ĭ���жϴ������ */
	static void DefaultExceptionHandler();		/* Ĭ���쳣������� */

public:
	/* �����жϺš��ж�/�쳣���������ڵ�ַ������IDT���ж�Ӧ���ж��������� */
	void SetInterruptGate(int number, unsigned int handler);
	
	/* �����жϺš��ж�/�쳣���������ڵ�ַ������IDT���ж�Ӧ�������������� */
	void SetTrapGate(int number, unsigned int handler);
	
	/* ����IDT�����ʼ��ַ(���Ե�ַ)�볤������GDTR�ṹ�� */
	void FormIDTR(IDTR& idtr);

private:
	GateDescriptor m_Descriptor[256];	/* 256 * 8 Bytes */
};

#endif
