using System;


namespace Build
{
	/// <summary>
	/// file ��ժҪ˵����
	/// </summary>
	public class File
    {
        #region ����
        public static  int CREATFILE = 1;
		public static  int DELETEFILE = 2;
        
		/*******i_mode********/
		public static  uint IALLOC = 0x8000;
		public static  uint IFMT = 0x6000;
		public static  uint IFDIR = 0x4000;
		public static  uint IFCHR = 0x2000;
		public static  uint IFBLK = 0x6000;
		public static  uint IREAD = 0x100;
		public static  uint IWRITE = 0x80;
		public static  uint IEXEC = 0x40;

        #endregion

        /// <summary>
        /// Ҫ��V6�д������ļ�·��
        /// </summary>
        protected string _createFilePath;
        /// <summary>
		/// Ŀ¼����
		/// </summary>
		protected Directary _dire;
		/// <summary>
		/// Ŀ¼��inode��
		/// </summary>
		protected int _dirInodeNo;
		/// <summary>
		/// ��Ŀ¼��inode��
		/// </summary>
		protected InodeStr _dirInode;
		/// <summary>
		/// �ļ���inode��
		/// </summary>
		protected InodeStr _fileInode;
		/// <summary>
		/// �ļ���inode��
		/// </summary>
		protected int _fileInodeNo;
		/// <summary>
		/// ����Ŀ¼���λ��,���ַ
		/// </summary>
		protected int _blankDirItem;
		/// <summary>
		/// ���������
		/// </summary>
		protected Superblock _superBlock;
		/// <summary>
		/// inode����
		/// </summary>
		protected InodeBlock _inodeblock;
		/// <summary>
		/// �������ݿ����
		/// </summary>
		protected DataBlock _dataBlock;
        /// <summary>
        /// �����ļ�
        /// </summary>
        protected Disk _diskFile;
		/// <summary>
		/// ���ڴ����namei��������ļ���(�������֣�����·������Ϊref ��������namei)
		/// </summary>
		protected char[] _fileName = new char[28];

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="tsb"></param>
        /// <param name="tid"></param>
        /// <param name="tdb"></param>
        /// <param name="diskInfo"></param>
        /// <param name="createFilePath"></param>
		public File(Superblock tsb,InodeBlock tid,DataBlock tdb,Disk diskInfo,string createFilePath)
		{
			_superBlock = tsb;
			_inodeblock = tid;
			_dataBlock = tdb;
            _diskFile = diskInfo;
			_dire = new Directary(tsb,tid,tdb,diskInfo);
			_fileInode = new InodeStr();
			_dirInode = new InodeStr();
            _createFilePath = createFilePath;
		}

        /// <summary>
        /// ����·���������ļ��Լ������Ϣд��
        /// </summary>
		public virtual void CreateFile()
		{
            //��ȡĿ¼inode
            SearchFatherDir(_createFilePath);
            if (Error.ErrorType != Error.SUCCESS)
            {
                return;
            }
            //��ддĿ¼��
            WriteDirItem(_dirInode);
            //д�½���inode����д�����
            //_inodeblock.CleanInodeAddr(_fileInode);
            SetCommonFileInode();
            _inodeblock.UpdateInodeToDisk(_fileInode, _fileInodeNo);
            //����Ŀ¼inode��д�ش���
            UpdateFatherDirInode();
            _inodeblock.UpdateInodeToDisk(_dirInode, _dirInodeNo);
        }

        /// <summary>
        /// ����·�������ļ�ϵͳ�н�������
        /// </summary>
        /// <param name="path"></param>
		protected void SearchFatherDir(string path)
		{
			_blankDirItem = _dire.NameI(path,ref _dirInodeNo,ref _dirInode,ref _fileInodeNo,ref _fileInode ,ref _fileName,CREATFILE);
   		}

		/// <summary>
		/// д��Ŀ¼��
		/// </summary>
		protected void WriteDirItem(InodeStr dirInode)
		{
			byte[] inodeNo;
			byte[] cin = new byte[32];
			inodeNo = Helper.Struct2Bytes(_fileInodeNo);

			for(int i = 0;i < 4;++i)
			{
				cin[i] = inodeNo[i];
			}
            //�˴���FileName��֮ǰ��namei�е�����ָ�����ļ��ľ�����������·��
			for(int i = 0;i < 28;++i)
			{
				cin[i + 4] = Convert.ToByte(_fileName[i]);
			}
			WriteStream(cin,_blankDirItem,32,dirInode);
		}

		/// <summary>
		/// ��д��
		/// </summary>
		/// <param name="cin">��д�����</param>
		/// <param name="position">д����ʼλ��</param>
		/// <param name="count">��д�������</param>
		protected void WriteStream(byte[] cin,int itemNum,int count,InodeStr dinode)
		{
            int pos = _dire.BMap(dinode, itemNum / 512);
            _diskFile.OpenFile();
			_diskFile.SeekFilePosition(pos * 512 + itemNum % 512,System.IO.SeekOrigin.Begin);
			_diskFile.WriteFile(ref cin,0,count);
            _diskFile.CloseFile();
		}

        /// <summary>
        /// �����ļ���inode����
        /// </summary>
		protected virtual void SetCommonFileInode()
		{
			_fileInode._i_ilink = 1;
			_fileInode._i_gid = 0;
			_fileInode._i_uid = 0;
			_fileInode._i_atime = Helper.GetCurrentTime();
            _fileInode._i_mtime = Helper.GetCurrentTime();
			_fileInode._i_size = 0;
		}

        /// <summary>
        /// ���¸�Ŀ¼��inode��Ϣ
        /// </summary>
		public void UpdateFatherDirInode()
		{
			_dirInode._i_size += 32;
            _dirInode._i_atime = Helper.GetCurrentTime();
            _dirInode._i_mtime = Helper.GetCurrentTime();
		}
	}
}
