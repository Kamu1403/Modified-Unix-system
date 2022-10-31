#include "PEParser.h"
#include "Utility.h"
#include "PageManager.h"

PEParser::PEParser(unsigned long peAddress)
{
	this->peAddress = peAddress + 0xC0000000;
}


unsigned long PEParser::Parse()
{
	ImageDosHeader* dosHeader = (ImageDosHeader*)this->peAddress;
	if( dosHeader->e_magic != 0x5a4d )
	{
		return 0xffffffff;
	}
	
	ImageNTHeader* ntHeader = 
		(ImageNTHeader*)(this->peAddress + dosHeader->e_lfanew);

	this->ntHeader = ntHeader;

	this->EntryPointAddress = 
		ntHeader->OptionalHeader.AddressOfEntryPoint + ntHeader->OptionalHeader.ImageBase;
	this->TextAddress = 
		ntHeader->OptionalHeader.BaseOfCode + ntHeader->OptionalHeader.ImageBase;
	this->TextSize = 
		ntHeader->OptionalHeader.BaseOfData - ntHeader->OptionalHeader.BaseOfCode;

	this->DataAddress =
		ntHeader->OptionalHeader.BaseOfData + ntHeader->OptionalHeader.ImageBase;

	this->StackSize = 
		ntHeader->OptionalHeader.SizeOfStackCommit;
	this->HeapSize =
		ntHeader->OptionalHeader.SizeOfHeapCommit;

	/*
	 * @comment 这里hardcode gcc的逻辑
	 * section 顺序为 .text->.data->.rdata->.bss
	 *
	 */
	unsigned offset = (unsigned long)ntHeader + sizeof(*ntHeader);
	this->sectionHeaders = (ImageSectionHeader*)offset;
	
	/*
	 * 现在只计算Data段的大小，假定个各类型的数据段连续存放，
	 * 因此只要知道.idata段的起始位置与大小，并对其到4k就是
	 * Data段大小
	 */
	ImageSectionHeader* lastDataHeader = 
		&(this->sectionHeaders[this->IDATA_SECTION_IDX]);
	
	this->DataSize = lastDataHeader->VirtualAddress - ntHeader->OptionalHeader.BaseOfData;
	return this->EntryPointAddress;
}

unsigned int PEParser::Relocate()
{
	unsigned long srcAddress, desAddress;
	unsigned cnt = 0;

	for (unsigned int i = 0; i < this->BSS_SECTION_IDX; i++ )
	{
		ImageSectionHeader* sectionHeader = &(this->sectionHeaders[i]);
		srcAddress = this->peAddress + sectionHeader->PointerToRawData;
		desAddress = 
			this->ntHeader->OptionalHeader.ImageBase + sectionHeader->VirtualAddress;
		Utility::MemCopy(srcAddress, desAddress, sectionHeader->Misc.VirtualSize);
		cnt += sectionHeader->Misc.VirtualSize;
	}

	return 	cnt;
}
