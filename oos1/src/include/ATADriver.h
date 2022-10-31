#ifndef ATA_DRIVE_H
#define ATA_DRIVE_H

#include "Regs.h"

class ATADriver
{
public:
	/* �����ж��豸�����ӳ��� */
	static void ATAHandler(struct pt_regs* reg, struct pt_context* context);

	/* ���ô��̼Ĵ������������̽���I/O���� */
	static void DevStart(struct Buf* bp);

private:
	/* ���������Ƿ����������ֵ�����ʾ�������ſ��Է������� */
	static int IsControllerReady();

	/* ���I/O�������Ƿ��д����� */
	static bool IsError();

	// /* Ŀǰδʵ��DMA����Ҫ�˺�����д����(����������Ҫ�˺���)ǰ��Ҫ
	//  * ȷ��Ӳ�̻������Ƿ�׼���ý������ݡ�˳�򣺴��̻��������� -> д��
	//  * ���̻����� -> ����д��Ŀ������ -> I/O��ɺ󷢳��жϡ�
	//  */
	// static int ReadyToTransfer();

	/* static const member */
public:
	/* ���̼Ĵ�������I/O�˿ڵ�ַ */
	static const unsigned short DATA_PORT	=	0x1f0;	/* ���ݼĴ����˿� */
	static const unsigned short ERROR_PORT	=	0x1f1;	/* ����Ĵ����˿� */
	static const unsigned short NSECTOR_PORT =	0x1f2;	/* �������Ĵ����˿� */
	static const unsigned short BLKNO_PORT_1 =	0x1f3;	/* �����żĴ����˿�(1)�����LBA28Ѱַģʽ�д��̿�ŵ�0-7λ */
	static const unsigned short BLKNO_PORT_2 =	0x1f4;	/* �����żĴ����˿�(2)�����LBA28Ѱַģʽ�д��̿�ŵ�8-15λ */
	static const unsigned short BLKNO_PORT_3 =	0x1f5;	/* �����żĴ����˿�(3)�����LBA28Ѱַģʽ�д��̿�ŵ�16-23λ */
	static const unsigned short MODE_PORT	=	0x1f6;	/* BIT(0-3)�������żĴ����˿�(4)�����LBA28Ѱַģʽ�д��̿�ŵ�24-27λ 
														   BIT(4-7)��ATA���̹���ģʽ��Bit(7��5)Ϊ1��ʾ��IDE�ӿڣ�Bit(6)Ϊ1��ʾ����LBA28ģʽ��Bit(4)Ϊ0��ʾ���̡�*/
	static const unsigned short CMD_PORT	=	0x1f7;	/* ֻд������Ĵ����˿� */
	static const unsigned short STATUS_PORT	=	0x1f7;	/* ֻ������״̬�Ĵ����˿� */
	static const unsigned short CTRL_PORT	=	0x3f6;	/* ���ƼĴ����˿ڣ���Ҫ���ڿ��ƴ���I/O������ɺ��Ƿ����ж� */
	
	/* ��״̬�Ĵ���(ֻ��)����λ���� (�˿ںţ�0x1f7) */
	static const unsigned char HD_ERROR	= 0x01;			/* ִ�����һ�δ��̲��������з������� */
	static const unsigned char HD_DEVICE_REQUEST = 0x08;	/* �������Ѿ�׼���ôӴ��̻��������ͺͽ������� */
	static const unsigned char HD_DEVICE_FAULT = 0x20;	/* ����������д���� */
	static const unsigned char HD_DEVICE_READY = 0x40;	/* ���̿������Ѿ�׼�����������Է��Ͷ���д���� */
	static const unsigned char HD_DEVICE_BUSY =	0x80;	/* ������æ������ִ����� */
	
	/* ����Ĵ���(ֻд)����λ���� (�˿ںţ�0x1f7) */
	static const unsigned char HD_READ	= 0x20;	/* ���̶���������������������� */
	static const unsigned char HD_WRITE	= 0x30;	/* ����д��������������������� */
	static const unsigned char HD_DMA_READ = 0xC8;	/* DMA��ʽ���������� */
	static const unsigned char HD_DMA_WRITE = 0xCA;	/* DMA��ʽд�������� */
	
	/* ����ģʽ�Ĵ�������λ���� (�˿ںţ� 0x1f6) */
	static const unsigned char MODE_IDE = 0xA0;		/* ATA���̹���ģʽ��Bit(7��5)Ϊ1��ʾ��IDE�ӿ� */
	static const unsigned char MODE_LBA28 = 0x40;	/* ATA���̹���ģʽ��Bit(6)Ϊ1��ʾ����LBA28ģʽ */
};

#endif
