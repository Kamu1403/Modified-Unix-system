#ifndef CHIP8253_H
#define CHIP8253_H

/*
 * �����8253�ɱ�̶�ʱоƬ(PIT)�Ĳ�����
 *
 * 8253оƬ���ڲ����̶������ʱ���жϡ�
 */
class Chip8253
{
public:
	/* ��8253ʱ��оƬ���г�ʼ����Ĭ��ÿ�����60��ʱ���ж� */
	static void Init(int ticks = 60); 

private:
	/* ������һЩ���ڶ˿ڵ�ַ������ֵ�ͼ���ֵ�ĳ��� */
	static const unsigned int INPUT_FREQ = 1193180;		/* оƬ����Ƶ��Ϊ1.193180MHz */
	static const unsigned short CNT0_PORT = 0x40;		/* ������0�Ķ˿ڵ�ַ40H */
	static const unsigned short CTRLWRD_PORT = 0x43;	/* �����ֶ˿ڵ�ַ43H */
	static const unsigned char CTRLCMD_SEL0 = 0x00;		/* ѡ�������0 */
	static const unsigned char CTRLCMD_MODE3 = 0x06;	/* ����ģʽ: ����̶����������ѭ���ķ���,��Ϊʱ���ж� */	
	static const unsigned char CTRLCMD_RW = 0x30;		/* ����ֵ��дģʽ: д��,16����,�ȵ��ֽ�,����ֽ� */
};

#endif
