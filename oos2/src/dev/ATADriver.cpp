#include "ATADriver.h"
#include "BufferManager.h"
#include "Utility.h"
#include "IOPort.h"
#include "Kernel.h"
#include "DMA.h"

void ATADriver::ATAHandler(struct pt_regs *reg, struct pt_context *context)
{
	Buf* bp;
	Devtab* atab;
	short major = Utility::GetMajor(DeviceManager::ROOTDEV);

	BlockDevice& bdev = 
		Kernel::Instance().GetDeviceManager().GetBlockDevice(major);
	atab = bdev.d_tab;
	
	if( atab->d_active == 0 )
	{
		return;		/* û�������� */
	}

	bp = atab->d_actf;		/* ��ȡ�����ж϶�Ӧ��I/O����Buf */
	atab->d_active = 0;		/* ��ʾ�豸�Ѿ����� */

	/* ���I/O����ִ�й����д��̿���������DMA�������Ƿ���� */
	if( ATADriver::IsError() || DMA::IsError() )
	{
		if(++atab->d_errcnt <= 10)
		{
			bdev.Start();
			return;
		}
		bp->b_flags |= Buf::B_ERROR;
	}
	
	atab->d_errcnt = 0;		/* ������������� */
	atab->d_actf = bp->av_forw;		/* ��I/O���������ȡ������ɵ�I/O����Buf */
	Kernel::Instance().GetBufferManager().IODone(bp);	/* I/O�����ƺ��� */
	bdev.Start();	/* ����I/O�����������һ��I/O���� */
	return;
}

void ATADriver::DevStart(struct Buf* bp)
{
	if(bp == NULL)
	{
		Utility::Panic("Invalid Buf in DevStart()!");
	}

	/* �ȴ��������ϳ�ʱ��δ��������ʾ���� */
	if(ATADriver::IsControllerReady() == 0)
	{
		Utility::Panic("Disk Hang Up!");
	}

	short minor = Utility::GetMinor(bp->b_dev);

	/* ��������������������(PRD Table) */
	PhysicalRegionDescriptor prd;
	static PRDTable table;
	
	/* �������ΪBuf��������ʵ�������ַ */
	/* 
	 * ���ڽ���ͼ����ʼ��ַp_addr�����û�̬�ռ��ַС��0xC0000000��
	 * ���Բ��ܲ��öԴ�����ͬ���ķ�����һ�����Ե�ַ��ȥ0xC0000000���ʵ�������ַ:
	 * prd.SetBaseAddress((unsigned long)bp->b_addr - 0xC0000000); //Oops~
	 * ����p_addr�д�ŵı�����Ѿ��������ַ�����Բ�ȡ���·����������֮�����򱣳֡�
	 */
	prd.SetBaseAddress((unsigned long)bp->b_addr & ~0xC0000000);
	prd.SetByteCount(bp->b_wcount);

	/* 
	 * ������õ�prd�������ŵ�PRD Table�ĵ�0��λ�ã�
	 * ���ұ��Ϊ���һ�(���������ʹ��һ��prd���������ɡ�) 
	 */
	table.SetPhysicalRegionDescriptor(0, prd, true);

	DMA::Reset();		/* ��λDMA������ */

	/* ���������� */
	IOPort::OutByte(ATADriver::NSECTOR_PORT, bp->b_wcount / BufferManager::BUFFER_SIZE);
	/* ����LBA28Ѱַģʽ�д��̿�ŵ�0-7λ */
	IOPort::OutByte(ATADriver::BLKNO_PORT_1, bp->b_blkno & 0xFF);
	/* ����LBA28Ѱַģʽ�д��̿�ŵ�8-15λ */
	IOPort::OutByte(ATADriver::BLKNO_PORT_2, (bp->b_blkno >> 8) & 0xFF);
	/* ����LBA28Ѱַģʽ�д��̿�ŵ�16-23λ */
	IOPort::OutByte(ATADriver::BLKNO_PORT_3, (bp->b_blkno >> 16) & 0xFF);
	/* ����ATA���̹���ģʽ�Ĵ������Լ�LBA28�е�24-27λ */
	IOPort::OutByte(ATADriver::MODE_PORT, ATADriver::MODE_IDE | ATADriver::MODE_LBA28 | (minor << 4) | ((bp->b_blkno >> 24) & 0x0F) );

	/* ����Ƕ����� */
	if( (bp->b_flags & Buf::B_READ) == Buf::B_READ )
	{
		/* ���ߴ��̿������Ķ���д���ͣ�����I/O */
		IOPort::OutByte(ATADriver::CMD_PORT, ATADriver::HD_DMA_READ);
		
		/* ����DMA�������Ķ���д���ͣ�����PRD Table��������ʼ��ַ������һ��DMA */
		DMA::Start(DMA::READ, table.GetPRDTableBaseAddress());
	}
	else	/* �����д���� */
	{
		IOPort::OutByte(ATADriver::CMD_PORT, ATADriver::HD_DMA_WRITE);
		
		DMA::Start(DMA::WRITE, table.GetPRDTableBaseAddress());
	}
	return;
}

int ATADriver::IsControllerReady()
{
	int ticks = 10000;
	
	while(--ticks)
	{
		unsigned char status = IOPort::InByte(ATADriver::STATUS_PORT);
		if( (status & (ATADriver::HD_DEVICE_BUSY | ATADriver::HD_DEVICE_READY)) == ATADriver::HD_DEVICE_READY )
		{
			return ticks;
		}
	}
	return 0;	/* ��������ʱ������Ӧ */
}

bool ATADriver::IsError()
{
	unsigned char status = IOPort::InByte(ATADriver::STATUS_PORT);
	if( (status & ATADriver::HD_ERROR) == ATADriver::HD_ERROR )
	{
		return true;	/* ���� */
	}
	return false;	/* û�г��� */
}
