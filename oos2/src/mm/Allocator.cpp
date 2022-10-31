#include "Allocator.h"

Allocator Allocator::m_Instance;

Allocator& Allocator::GetInstance()
{
	return Allocator::m_Instance;
}

//�״���Ӧ�㷨
unsigned long Allocator::Alloc(MapNode map[], unsigned long size)
{
	MapNode* pNode;
	unsigned long retIdx = 0;

	/* ��pNode->m_Size == 0���ʾ�Ѿ���������β */
	for ( pNode = map; pNode->m_Size; pNode++)
	{
		if ( pNode->m_Size >= size )
		{
			retIdx = pNode->m_AddressIdx;
			pNode->m_AddressIdx += size;
			pNode->m_Size -= size;
			/* ��ǰ�ڴ����÷�����ɣ�����MapNode����λ�ú����MapNode����ǰ�ƶ�һ��λ�� */
			if ( pNode->m_Size == 0 )
			{
				MapNode* pNextNode = (pNode + 1);
				for ( ; pNextNode->m_Size; ++pNode, ++pNextNode)
				{
					pNode->m_AddressIdx = pNextNode->m_AddressIdx;
					pNode->m_Size = pNextNode->m_Size;
				}
				pNode->m_AddressIdx = pNode->m_Size = 0;
			}
			return retIdx;
		}
	}
	/* no match found */
	return 0;
}

//ѭ���״���Ӧ�㷨
unsigned long Allocator::Alloc(MapNode map[], unsigned long size, MapNode* &beginNode)
{
	MapNode* pNode;
	unsigned long retIdx = 0;

	//�����α������̣����ȴ�beginNode��Map��β����δӿ�ʼ��beginNode��ǰһ����ֱ���ҵ�һ�����ʵĿ�
	//��һ��
	for(pNode = beginNode; pNode->m_Size; pNode++)
		if(pNode->m_Size < size)
			continue;
		else
			break;

	//�����ʾ�ڵ�һ�α���������û�ҵ����ʵĿ飬���Կ�ʼ�ڶ��α���
	if(pNode->m_Size == 0){
		//�ڶ��α���
		for(pNode = map; pNode != beginNode; pNode++)
			if(pNode->m_Size < size)
				continue;
			else
				break;

		//���ﵽ������յ㣬��ʾû�ҵ��飬����0
		if(pNode == beginNode)
			/* no match found */
			return 0;
	}

	//��ʱ��û���أ���˵���ҵ����ʵĿ�
	retIdx = pNode->m_AddressIdx;
	pNode->m_AddressIdx += size;
	pNode->m_Size -= size;

	/* ��ǰ�ڴ����÷�����ɣ�����MapNode����λ�ú����MapNode����ǰ�ƶ�һ��λ�� */
	if(pNode->m_Size == 0){
		beginNode = pNode; //pNode��������һ�����滻������pNodeֱ����Ϊ��һ����Ӧ�Ŀ�ʼ
		MapNode* pNextNode = (pNode + 1);
		for ( ; pNextNode->m_Size; ++pNode, ++pNextNode){
			pNode->m_AddressIdx = pNextNode->m_AddressIdx;
			pNode->m_Size = pNextNode->m_Size;
		}
		pNode->m_AddressIdx = pNode->m_Size = 0;
	}
	else
		beginNode = pNode + 1; //����pNode����һ���Ϊ��һ����Ӧ�Ŀ�ʼ

	return retIdx; //������ʼ��ַ

}

unsigned long Allocator::Free(MapNode map[], unsigned long size, unsigned long addrIdx)
{
	MapNode* pNode;
	/* ���ȣ�pNodeָ��������addrIdx��һ������� */
	for ( pNode = map; pNode->m_AddressIdx <= addrIdx && pNode->m_Size != 0; ++pNode );
	/* 
	 * 1) pNode���ǵ�һ�飬��������ͷ������
	 * 2) ��Ҫfree�����ݿ�������pLastNode����
	 * 3) ��Ϊ���pNode�ǵ�һ��������2)һ�����������
	 * ��˿��Ա�֤pLastNode��������һ������С��map�ĵ�ַ
	 */
	MapNode* pLastNode = pNode - 1;
	if ( pNode > map && addrIdx == pLastNode->m_AddressIdx + pLastNode->m_Size )
	{
		pLastNode->m_Size += size;
		/* ���ﴦ���������п����ڵ��������Ҫ�ϲ���������ǰ�ƶ����п� */
		if ( addrIdx + size == pNode->m_AddressIdx )  
		{
			pLastNode->m_Size += pNode->m_Size;
			for ( ++pLastNode, ++pNode; pNode->m_Size; ++pLastNode, ++pNode )
			{
				pLastNode->m_AddressIdx = pNode->m_AddressIdx;
				pLastNode->m_Size = pNode->m_Size;				
			}
			pLastNode->m_AddressIdx = pLastNode->m_Size = 0;
		}
	}
	/* ���ﴦ���������
	 * 1) ������pNode������pNode������Ч�ģ�ֻҪ�޸�pNode��AddressIdx���Լ���
	 * 2) ǰ�󶼲����ڣ�����Ҫ��pNode���Ժ�Ľڵ���������ƶ�
	 */
	else
	{
		if ( addrIdx + size == pNode->m_AddressIdx && pNode->m_Size )
		{
			pNode->m_AddressIdx = addrIdx;
			pNode->m_Size += size;
		}
		else if ( size ) //�Ϸ����ж�
		{
			//��������2)�����
			MapNode tmpNode1, tmpNode2;
			tmpNode1.m_AddressIdx = addrIdx;
			tmpNode1.m_Size = size;

			for ( ; pNode->m_Size; ++pNode )
			{
				tmpNode2.m_AddressIdx = pNode->m_AddressIdx;
				tmpNode2.m_Size = pNode->m_Size;

				pNode->m_AddressIdx = tmpNode1.m_AddressIdx;
				pNode->m_Size = tmpNode1.m_Size;

				tmpNode1.m_AddressIdx = tmpNode2.m_AddressIdx;
				tmpNode1.m_Size = tmpNode2.m_Size;
			}
			/* �����һ������pNode�� */
			pNode->m_AddressIdx = tmpNode1.m_AddressIdx;
			pNode->m_Size = tmpNode1.m_Size;
		}
	}
	return 0;
}

