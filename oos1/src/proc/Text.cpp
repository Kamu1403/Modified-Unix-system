#include "Text.h"
#include "Kernel.h"

Text::Text()
{
	//nothing to do here
}

Text::~Text()
{
	//nothing to do here
}

void Text::XccDec()
{
	if ( this->x_ccount == 0 )
		return;

	/* ���x_ccount�ݼ���0�����ͷŸù������Ķ�ռ�ݵ��ڴ档*/
	if ( --this->x_ccount == 0 )
	{
		Kernel::Instance().GetUserPageManager().FreeMemory(this->x_size, this->x_caddr);
	}
}

void Text::XFree()
{
	this->XccDec();
	/* 
	 * ������øù������ĶεĽ�����Ϊ0�����̶�����ֹ
	 * ��û�б�Ҫ�ڽ������ϱ��湲�����Ķεĸ�����
	 */
	if ( --this->x_count == 0 )
	{
		Kernel::Instance().GetSwapperManager().FreeSwap(this->x_size, this->x_daddr);
		Kernel::Instance().GetFileManager().m_InodeTable->IPut(this->x_iptr);
		this->x_iptr = NULL;
	}
}
