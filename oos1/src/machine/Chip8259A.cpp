#include "Chip8259A.h"
#include "IOPort.h"

void Chip8259A::Init()
{
	/* Initialize 8259A */
	//���ICW1ȷ��������Ƭ�Ĺ���ģʽΪ
	//����ģʽ�����ش�������Ҫ���ICW4
	IOPort::OutByte(MASTER_IO_PORT_1, 0x11);
	IOPort::OutByte(SLAVE_IO_PORT_1, 0x11);
	
	//���ICW2ȷ��������Ƭ��Ӧ���ж�������
	IOPort::OutByte(MASTER_IO_PORT_2, MASTER_IRQ_START);
	IOPort::OutByte(SLAVE_IO_PORT_2,  SLAVE_IRQ_START);
	
	//���ICW3ȷ��������Ƭ�������������
	/* TO MASTER:  ��Ƭ��IR2���������˴�Ƭ,����IR2��Ӧ����λΪ1 ��*/
	IOPort::OutByte(MASTER_IO_PORT_2, 1 << IRQ_SLAVE); /* 0x04 = 0000 0100b */
	/* TO SLAVE: ��Ƭ���ӵ���Ƭ��IR2����, ����Ҫʹ��D2~D0 ���� 2��*/
	IOPort::OutByte(SLAVE_IO_PORT_2, IRQ_SLAVE);
	
	//���ICW4ȷ��������Ƭ���ж�ִ��ģʽ
	//��ͨȫǶ��ģʽ������EOI(End of Interrupt)��Intel X86ģʽ
	IOPort::OutByte(MASTER_IO_PORT_2, 0x01);
	IOPort::OutByte(SLAVE_IO_PORT_2, 0x01);
	
	/* ��ʼ��֮�������������ж�,дOCW1�Ĵ���,���ж�����λ����һ����֪״̬�¡�*/
	IOPort::OutByte(MASTER_IO_PORT_2, MASK_ALL);
	IOPort::OutByte(SLAVE_IO_PORT_2,  MASK_ALL);	
	return;
}


void Chip8259A::IrqEnable(unsigned int irq)
{
	/* 
	 * IRQ��Χ��0��15֮�ڣ�irq��unsigned int���ͣ�С��0�����ܣ�
	 * ��ʵ����irq����ֵӦ��ֻ����IRQ_TIMER��IRQ_KBD���Ѿ������
	 * �ļ���������IRQ�ţ���IRQ6�����������ж�֮�����ں��в�δ�漰��
	 */
	if(irq >= 16)
		{return; /*.....���������Ϣ*/}
	
	unsigned char value;
	
	if(irq <= 7) /* 0 <= irq <= 7,  ��MASTER chip��OCW1����*/
	{
		// Get ICW1
		value = IOPort::InByte(MASTER_IO_PORT_2);
		value &= ~(1 << irq);
		//Write ICW1 back to 8259A
		IOPort::OutByte(MASTER_IO_PORT_2, value);
	}
	else /*��SLAVE chip��OCW1����*/
	{
		value = IOPort::InByte(SLAVE_IO_PORT_2);
		value &= ~(1 << (irq - 8));
		IOPort::OutByte(SLAVE_IO_PORT_2, value);
	}
	
	return;
}

void Chip8259A::IrqDisable(unsigned int irq)
{
	if(irq >= 16)
		{return; /*.....���������Ϣ*/}
	
	unsigned char value;
	
	if(irq <= 7)	/* 0 <= irq <= 7,  ��MASTER chip��OCW1����*/
	{
		value = IOPort::InByte(MASTER_IO_PORT_2);
		value |= (1 << irq);
		IOPort::OutByte(MASTER_IO_PORT_2, value);
	}
	else /*��SLAVE chip��OCW1����*/
	{
		value = IOPort::InByte(SLAVE_IO_PORT_2);
		value |= (1 << (irq - 8));
		IOPort::OutByte(SLAVE_IO_PORT_2, value);
	}
	return;
}
