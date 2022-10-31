#ifndef DMA_H
#define DMA_H

/* 
 * Physical Region Descriptor(PRD) �����ڴ���������
 * �������������ڴ�������֮�����DMA��ʽ���ݴ���ʱ��
 * Դ(��Ŀ��)�����ڴ�������ʼ��ַ�� ������Ϣ�����ݽṹ��
 */
class PhysicalRegionDescriptor
{
	/* Members */
private:
	unsigned long m_BaseAddressZeroBit : 1;	/*  �ڴ����� "������ʼ��ַ"(ע���������Ե�ַ)�ĵ�0λ����λ����Ϊ0����ζ���ڴ滺��������2�ֽڶ��� */
	unsigned long m_MemoryRegionPhysicalBaseAddress : 31;	/* �ڴ����� "������ʼ��ַ"�ĵ�[31 : 1]λ */
	unsigned short m_ByteCountZeroBit : 1;			/* DMA�����ֽ���Byte Count�ĵ�0λ����Ϊ0�� ��ζ��ÿ�δ�����ֽ������������������ֽڳ��� */
	unsigned short m_ByteCount : 15;				/* DMA�����ֽ���Byte Count�ĵ�[15 : 1]λ */
	unsigned short m_Reserved : 15;					/* �������� */
	unsigned short m_EOT : 1;						/* Bit(31)��End of Tableλ����λΪ1��ʾ��ǰ�����ڴ���������(PRD)��PRD���е����һ� */
	
public:
	void SetBaseAddress(unsigned long phyBaseAddr);
	void SetByteCount(unsigned short bytes);
	void SetEndOfTable(bool EOT /* End of Table */ );
	
}__attribute__((packed));


/* 
 * �����ڴ���������(PRD)��һ������PRD���Թ�����������
 * ÿ����������һ������DMA����������ڴ������ɴ˿�������
 * �����ڴ��ϲ������Ķ����������ͬһ��DMA���䡣
 * 
 * �������һ�������ڴ�����������Bit(31)Ϊ1��ʾPRD Table�Ľ�����
 * ÿ����һ��DMA����ʱ��DMA����оƬ��PRD Table�ĵ�0�ʼ������
 * ��/д����ÿһ��PRD�������ڴ�����ֱ��DMAоƬ��⵽���е�n��PRD
 * ��Bit(31)Ϊ1������ΪPRD Table�������������DMA���䡣
 * 
 * ע: PRD Table�����������������������������������ڴ��������ǲ�����
 * �ģ���������������������������ڴ����������ģ����PRDTable��
 * ʹ���������ʽ��ʵ����������
 */
class PRDTable
{
public:
	/* static const */
	static const int NSIZE = 10;		/* PRD Table�������������������Ŀ */

	/* Members */
public:
	/* ����index��Ӧ�������ڴ���������(PRD) */
	void SetPhysicalRegionDescriptor(int index, PhysicalRegionDescriptor& prd, bool EOT /* End of Table */);
	
	/* ��ȡPRD Table��������ʼ��ַ (ע�����ص��������ַ�����������Ե�ַ) */
	unsigned long GetPRDTableBaseAddress();
	
private:
	/* 
	 * PRD Table����ʼ��ַ����4�ֽڶ��룬��������DMA����оƬ�ڲ���
	 * ����������������Ĵ���(PRDTR)���üĴ������PRD Table��Base Address��
	 * ��[31 : 2]λ,�����Ե�[1 : 0]��λ�������Ҫ4�ֽڶ��롣
	 */
	PhysicalRegionDescriptor m_Descriptors[NSIZE] __attribute__((aligned (4)));
};


/* 
 * DMA���װ��DMA����оƬ���ڲ��Ĵ����Ķ˿ںţ�
 * ��������Ĵ�����״̬�Ĵ������Լ���������
 * ��������Ĵ���(PRDTR)�Ķ˿ںš�
 * 
 * ͬʱ������Щ�Ĵ������ض�����λ������Ӧ�ĳ�����
 */
class DMA
{
public:
	/* static member */
	static unsigned short COMMAND_PORT;		/* ����Ĵ����Ķ˿ں� */
	static unsigned short STATUS_PORT;		/* ״̬�Ĵ����Ķ˿ں� */
	static unsigned short PRDTR_PORT;		/* PRD Table����ַ�Ĵ����Ķ˿ں� */

	/* ����Ĵ��� (�˿ںţ�COMMAND_PORT) ����λ���� */
	enum DMAStart	/* ����Ĵ���Bit(0)��Start/Stopλ */
	{
		START	=	0x01,		/* Bit(0) = 1������һ��DMA */
		STOP	=	0x00		/* Bit(0) = 0��ֹͣ����ִ�е�DMA��
								��ִ����ǰһ��DMA֮��Ҳ��Ҫ�����ָ�Bit(0)��Ϊ0 */
	};

	enum DMAType	/* ����Ĵ���Bit(3), Read/Writeλ������DMA����оƬ����DMA����ķ��� */
	{
		READ	=	0x08,		/* DMA Read��Bit(3) = 1����ʾ��Ӳ�̣�д���ڴ� */
		WRITE	=	0x00		/* DMA Write��Bit(3) = 0����ʾдӲ�̣����ڴ� */
	};
	/* ����Ĵ��� (�˿ںţ�COMMAND_PORT) 8������ֻ��Bit(0)��Bit(3)Ϊ��Чλ�� */


	/* ״̬�Ĵ��� (�˿ںţ�STATUS_PORT) ����λ���壺 ֻ�õ�8������λ�е�Bit(0 - 2) */
	static const unsigned char ACTIVE = 0x01;	/* AcitveλBit(0)��DMA����ִ���ڼ�Ӳ����1��
												DMA��ɺ���Ӳ���Զ���0��*/
												
	static const unsigned char ERROR =	0x02;	/* ErrorλBit(1)��DMAִ�й������������Ӳ������λ��1��
												���ָ��ͨ��д��һ����1�����ñ���λ��λ��0��
												ͨ��д��1�������㣬�е���֡��� ���Ǿ�����Ч��*/
												
	static const unsigned char Interrupt = 0x04; /* InterruptλBit(2)����DMA������ɣ�
												������������CPU�����ж�����(����CPU�Ƿ���ж�)֮��
												��Ӳ����Ϊ1����ʾ�Ѿ������ж����� 
												���ָ��ͨ��д��һ����1�����ñ���λ��λ��0��*/

public:
	static void Init();			/* ��ʼ��DMAоƬ��ȷ��DMA����оƬ�ڲ��Ĵ�����ռ�ݵ�I/O�˿ں� */

	static void Reset();		/* ����DMA����оƬ�����ǰһ��DMA����Ľ��״̬ */
	
	static bool IsError();		/* ���ǰһ��DMAִ�й������Ƿ���� */

	/* ���ݲ����涨��DMA���͡�PRD Table����ʼ�����ַ������DMA���� */
	static void Start(enum DMAType type, unsigned long baseAddress);
};

#endif
