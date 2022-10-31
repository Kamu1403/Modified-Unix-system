using System;
using System.Collections.Generic;
using System.Text;

namespace MakeImage
{
    class Constant
    {
        /* ����һЩ���� */
        
        /* �ַ����С */
        public static int BLOCK_SIZE = 512;

        /* ����SuperBlockλ�ڴ����ϵ������ţ�ռ��100��101���������� */
        public static int SUPER_BLOCK_SECTOR_NUMBER = 200;

        /* �ļ�ϵͳ��Ŀ¼���Inode��� */
        public static int ROOTINO = 0;

        /* ���INode���󳤶�Ϊ64�ֽڣ�ÿ�����̿���Դ��512/64 = 8�����Inode */
        public static int INODE_NUMBER_PER_SECTOR = 8;

        /* ���Inode��λ�ڴ����ϵ���ʼ������ */
        public static int INODE_ZONE_START_SECTOR = 202;

	    /* ���������Inode��ռ�ݵ������� */
        public static int INODE_ZONE_SIZE = 1024 - 202;

        /* ����������ʼ������ */
	    public static int DATA_ZONE_START_SECTOR = 1024;

        /* C.img�������� */
        public static int NSECTOR = 20160;

        /* exe Start Block */
        public static int EXEBLOCK = 19000;

	    /* �������Ľ��������� */
        public static int DATA_ZONE_END_SECTOR = 18000 - 1;

        /* ������ռ�ݵ��������� */
        public static int DATA_ZONE_SIZE = 18000 - DATA_ZONE_START_SECTOR;
    }
}
