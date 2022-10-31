#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "MapNode.h"

/* @comment ����Ϊ�ڴ�����㷨�࣬���ʹ��MapNode
 * �����ǵ��������������PageManager��SwapDiskManager��
 * ���к�����Unixv6�ж�Ӧ��ϵ���£�
 * Alloc()	: malloc(mp, size)		@line 2538
 * Free()	: mfree(mp, size, aa)	@line 2556 
 */
class Allocator
{
/* Functions */
public:
	unsigned long Alloc(MapNode map[], unsigned long size);
	unsigned long Free(MapNode map[], unsigned long size, unsigned long addrIdx);

public:
	static Allocator& GetInstance();
private:
	static Allocator m_Instance;
};

#endif

