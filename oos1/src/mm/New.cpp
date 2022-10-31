#include "New.h"
/*
 * ����ļ�������
 * 1�����ص��ǿռ�������ַ
 * 2��new����Ĳ�������Ҫ�Ŀռ��С��û������ɱ䳤���飬�μ�PEParser.cpp��  line 157.
 */

KernelAllocator* g_pAllocator;

void set_kernel_allocator(KernelAllocator* pAllocator)
{
	g_pAllocator = pAllocator;
}

void* operator new (unsigned int size)
{

	unsigned long address = g_pAllocator->AllocMemory(size + sizeof(int));
	if ( address )
	{
		int* pSize = (int*)address;
		*pSize = size;
		return (void*)(address + sizeof(int));
	}
	else
	{
		return 0;
	}
}

void operator delete (void* p)
{
	unsigned long address = (unsigned long)p;
	if ( address )
	{
		int* pSize = (int*)(address - sizeof(int));
		g_pAllocator->FreeMemeory(*pSize + sizeof(int), (unsigned long)pSize);
	}
	return;
}

