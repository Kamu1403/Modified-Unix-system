#ifndef MAP_NODE_H
#define MAP_NODE_H

/*
 *@comment 这个结构对应Unixv6中的map结构
 *下面给出map结构参考
 * struct map	@line 2515
 * {
	char *m_size;
	char *m_addr;
 * }
 */
struct MapNode
{
	unsigned long m_Size;
	/* 
	 * m_addr 表示数据块在整个空间中的索引位置，
	 * 例如physical内存中4k一块，若m_AddressIdx为2，
	 * 则表示0x2000(8k)的位置，同理swap区中，
	 * 数据块大小为512byte 
	 */
	unsigned long m_AddressIdx;	
};

#endif

