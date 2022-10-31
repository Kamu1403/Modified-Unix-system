#ifndef SWAPPER_MANAGER_H
#define SWAPPER_MANAGER_H

#include "MapNode.h"
#include "Allocator.h"

class SwapperManager
{
public:
	/* static non-const member */
	/* !hard code!�趨���̴�18000#��ʼ��2000��������Ϊ������ */
	static unsigned int SWAPPER_ZONE_START_BLOCK;
	static unsigned int SWAPPER_ZONE_SIZE;

	/* static const member */
	static const unsigned int SWAPPER_MAP_ARRAY_SIZE = 0x200;
	static const unsigned int BLOCK_SIZE = 512;

	/* Functions */
public:
	SwapperManager(Allocator* pAllocator);
	~SwapperManager();

	/* ��ʼ��MapNode map[0]Ϊ���̽�������ʼ��ַ����С */
	int Initialize();
	/* 
	 * �������ռ����
	 * 
	 * size:  ������佻������С(��λ: byte)��ʵ�ʷ���Ľ�������С��
	 * ���̿�Ϊ��λ�����ݲ���size��С������ȡ�������̿��С����������
	 * 
	 * ����ֵ: ���䵽�Ľ�������ʼ�̿�ţ�����0��ʾ����ʧ�ܡ�
	 */
	int AllocSwap(unsigned long size);
	/* 
	 * �������ռ��ͷ�
	 * 
	 * size:  �ͷŽ�������С(��λ: byte)��ʵ���ͷŵĽ�������С��
	 * ���̿�Ϊ��λ�����ݲ���size��С������ȡ�������̿��С����������
	 * startBlock: �ͷŽ�������ʼ�̿��
	 * 
	 * ����ֵ: �ͷŽ������������ܳɹ�������0����ͨ�������䷵��ֵ��
	 */
	int FreeSwap(unsigned long size, int startBlock);

private:
	SwapperManager();

	/* Members */
public:
	MapNode map[SwapperManager::SWAPPER_MAP_ARRAY_SIZE];

private:
	Allocator* m_pAllocator;
};

#endif
