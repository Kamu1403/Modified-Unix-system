#include "FileSystem.h"
#include "Utility.h"
#include "New.h"
#include "Kernel.h"
#include "OpenFileManager.h"
#include "TimeInterrupt.h"
#include "Video.h"

/*==============================class SuperBlock===================================*/
/* ϵͳȫ�ֳ�����SuperBlock���� */
SuperBlock g_spb;

SuperBlock::SuperBlock()
{
	//nothing to do here
}

SuperBlock::~SuperBlock()
{
	//nothing to do here
}

/*==============================class Mount===================================*/
Mount::Mount()
{
	this->m_dev = -1;
	this->m_spb = NULL;
	this->m_inodep = NULL;
}

Mount::~Mount()
{
	this->m_dev = -1;
	this->m_inodep = NULL;
	//�ͷ��ڴ�SuperBlock����
	if(this->m_spb != NULL)
	{
		delete this->m_spb;
		this->m_spb = NULL;
	}
}

/*==============================class FileSystem===================================*/
FileSystem::FileSystem()
{
	//nothing to do here
}

FileSystem::~FileSystem()
{
	//nothing to do here
}

void FileSystem::Initialize()
{
	this->m_BufferManager = &Kernel::Instance().GetBufferManager();
	this->updlock = 0;
}

void FileSystem::LoadSuperBlock()
{
	User& u = Kernel::Instance().GetUser();
	BufferManager& bufMgr = Kernel::Instance().GetBufferManager();
	Buf* pBuf;

	for (int i = 0; i < 2; i++)
	{
		int* p = (int *)&g_spb + i * 128;

		pBuf = bufMgr.Bread(DeviceManager::ROOTDEV, FileSystem::SUPER_BLOCK_SECTOR_NUMBER + i);

		Utility::DWordCopy((int *)pBuf->b_addr, p, 128);

		bufMgr.Brelse(pBuf);
	}
	if (User::NOERROR != u.u_error)
	{
		Utility::Panic("Load SuperBlock Error....!\n");
	}

	this->m_Mount[0].m_dev = DeviceManager::ROOTDEV;
	this->m_Mount[0].m_spb = &g_spb;

	g_spb.s_flock = 0;
	g_spb.s_ilock = 0;
	g_spb.s_ronly = 0;
	g_spb.s_time = Time::time;
}

SuperBlock* FileSystem::GetFS(short dev)
{
	SuperBlock* sb;
	
	/* ����ϵͳװ����Ѱ���豸��Ϊdev���豸���ļ�ϵͳ��SuperBlock */
	for(int i = 0; i < FileSystem::NMOUNT; i++)
	{
		if(this->m_Mount[i].m_spb != NULL && this->m_Mount[i].m_dev == dev)
		{
			/* ��ȡSuperBlock�ĵ�ַ */
			sb = this->m_Mount[i].m_spb;
			if(sb->s_nfree > 100 || sb->s_ninode > 100)
			{
				sb->s_nfree = 0;
				sb->s_ninode = 0;
			}
			return sb;
		}
	}

	Utility::Panic("No File System!");
	return NULL;
}

void FileSystem::Update()
{
	int i;
	SuperBlock* sb;
	Buf* pBuf;

	/* ��һ�������ڽ���ͬ������ֱ�ӷ��� */
	if(this->updlock)
	{
		return;
	}

	/* ����Update()�����Ļ���������ֹ������������ */
	this->updlock++;

	/* ͬ��SuperBlock������ */
	for(i = 0; i < FileSystem::NMOUNT; i++)
	{
		if(this->m_Mount[i].m_spb != NULL)	/* ��Mountװ����Ӧĳ���ļ�ϵͳ */
		{
			sb = this->m_Mount[i].m_spb;

			/* �����SuperBlock�ڴ渱��û�б��޸ģ�ֱ�ӹ���inode�Ϳ����̿鱻��������ļ�ϵͳ��ֻ���ļ�ϵͳ */
			if(sb->s_fmod == 0 || sb->s_ilock != 0 || sb->s_flock != 0 || sb->s_ronly != 0)
			{
				continue;
			}

			/* ��SuperBlock�޸ı�־ */
			sb->s_fmod = 0;
			/* д��SuperBlock�����ʱ�� */
			sb->s_time = Time::time;

			/* 
			 * Ϊ��Ҫд�ص�������ȥ��SuperBlock����һ�黺�棬���ڻ�����СΪ512�ֽڣ�
			 * SuperBlock��СΪ1024�ֽڣ�ռ��2��������������������Ҫ2��д�������
			 */
			for(int j = 0; j < 2; j++)
			{
				/* ��һ��pָ��SuperBlock�ĵ�0�ֽڣ��ڶ���pָ���512�ֽ� */
				int* p = (int *)sb + j * 128;

				/* ��Ҫд�뵽�豸dev�ϵ�SUPER_BLOCK_SECTOR_NUMBER + j������ȥ */
				pBuf = this->m_BufferManager->GetBlk(this->m_Mount[i].m_dev, FileSystem::SUPER_BLOCK_SECTOR_NUMBER + j);

				/* ��SuperBlock�е�0 - 511�ֽ�д�뻺���� */
				Utility::DWordCopy(p, (int *)pBuf->b_addr, 128);

				/* ���������е�����д�������� */
				this->m_BufferManager->Bwrite(pBuf);
			}
		}
	}
	
	/* ͬ���޸Ĺ����ڴ�Inode����Ӧ���Inode */
	g_InodeTable.UpdateInodeTable();

	/* ���Update()������ */
	this->updlock = 0;

	/* ���ӳ�д�Ļ����д�������� */
	this->m_BufferManager->Bflush(DeviceManager::NODEV);
}

Inode* FileSystem::IAlloc(short dev)
{
	SuperBlock* sb;
	Buf* pBuf;
	Inode* pNode;
	User& u = Kernel::Instance().GetUser();
	int ino;	/* ���䵽�Ŀ������Inode��� */

	/* ��ȡ��Ӧ�豸��SuperBlock�ڴ渱�� */
	sb = this->GetFS(dev);

	/* ���SuperBlock����Inode����������˯�ߵȴ������� */
	while(sb->s_ilock)
	{
		u.u_procp->Sleep((unsigned long)&sb->s_ilock, ProcessManager::PINOD);
	}

	/* 
	 * SuperBlockֱ�ӹ���Ŀ���Inode�������ѿգ�
	 * ���뵽��������������Inode���ȶ�inode�б�������
	 * ��Ϊ�����³����л���ж��̲������ܻᵼ�½����л���
	 * ���������п��ܷ��ʸ����������ᵼ�²�һ���ԡ�
	 */
	if(sb->s_ninode <= 0)
	{
		/* ����Inode���������� */
		sb->s_ilock++;

		/* ���Inode��Ŵ�0��ʼ���ⲻͬ��Unix V6�����Inode��1��ʼ��� */
		ino = -1;

		/* ���ζ������Inode���еĴ��̿飬�������п������Inode���������Inode������ */
		for(int i = 0; i < sb->s_isize; i++)
		{
			pBuf = this->m_BufferManager->Bread(dev, FileSystem::INODE_ZONE_START_SECTOR + i);

			/* ��ȡ��������ַ */
			int* p = (int *)pBuf->b_addr;

			/* ���û�������ÿ�����Inode��i_mode != 0����ʾ�Ѿ���ռ�� */
			for(int j = 0; j < FileSystem::INODE_NUMBER_PER_SECTOR; j++)
			{
				ino++;

				int mode = *( p + j * sizeof(DiskInode)/sizeof(int) );

				/* �����Inode�ѱ�ռ�ã����ܼ������Inode������ */
				if(mode != 0)
				{
					continue;
				}

				/* 
				 * ������inode��i_mode==0����ʱ������ȷ��
				 * ��inode�ǿ��еģ���Ϊ�п������ڴ�inodeû��д��
				 * ������,����Ҫ���������ڴ�inode���Ƿ�����Ӧ����
				 */
				if( g_InodeTable.IsLoaded(dev, ino) == -1 )
				{
					/* �����Inodeû�ж�Ӧ���ڴ濽��������������Inode������ */
					sb->s_inode[sb->s_ninode++] = ino;

					/* ��������������Ѿ�װ�����򲻼������� */
					if(sb->s_ninode >= 100)
					{
						break;
					}
				}
			}

			/* �����Ѷ��굱ǰ���̿飬�ͷ���Ӧ�Ļ��� */
			this->m_BufferManager->Brelse(pBuf);

			/* ��������������Ѿ�װ�����򲻼������� */
			if(sb->s_ninode >= 100)
			{
				break;
			}
		}
		/* ����Կ������Inode�����������������Ϊ�ȴ�����˯�ߵĽ��� */
		sb->s_ilock = 0;
		Kernel::Instance().GetProcessManager().WakeUpAll((unsigned long)&sb->s_ilock);
		
		/* ����ڴ�����û���������κο������Inode������NULL */
		if(sb->s_ninode <= 0)
		{
			Diagnose::Write("No Space On %d !\n", dev);
			u.u_error = User::ENOSPC;
			return NULL;
		}
	}

	/* 
	 * ���沿���Ѿ���֤������ϵͳ��û�п������Inode��
	 * �������Inode�������бض����¼�������Inode�ı�š�
	 */
	while(true)
	{
		/* ��������ջ������ȡ�������Inode��� */
		ino = sb->s_inode[--sb->s_ninode];

		/* ������Inode�����ڴ� */
		pNode = g_InodeTable.IGet(dev, ino);
		/* δ�ܷ��䵽�ڴ�inode */
		if(NULL == pNode)
		{
			return NULL;
		}

		/* �����Inode����,���Inode�е����� */
		if(0 == pNode->i_mode)
		{
			pNode->Clean();
			/* ����SuperBlock���޸ı�־ */
			sb->s_fmod = 1;
			return pNode;
		}
		else	/* �����Inode�ѱ�ռ�� */
		{
			g_InodeTable.IPut(pNode);
			continue;	/* whileѭ�� */
		}
	}
	return NULL;	/* GCC likes it! */
}

void FileSystem::IFree(short dev, int number)
{
	SuperBlock* sb;

	sb = this->GetFS(dev);	/* ��ȡ��Ӧ�豸��SuperBlock�ڴ渱�� */
	
	/* 
	 * ���������ֱ�ӹ���Ŀ���Inode��������
	 * ���ͷŵ����Inodeɢ���ڴ���Inode���С�
	 */
	if(sb->s_ilock)
	{
		return;
	}

	/* 
	 * ���������ֱ�ӹ���Ŀ������Inode����100����
	 * ͬ�����ͷŵ����Inodeɢ���ڴ���Inode���С�
	 */
	if(sb->s_ninode >= 100)
	{
		return;
	}

	sb->s_inode[sb->s_ninode++] = number;

	/* ����SuperBlock���޸ı�־ */
	sb->s_fmod = 1;
}

Buf* FileSystem::Alloc(short dev)
{
	int blkno;	/* ���䵽�Ŀ��д��̿��� */
	SuperBlock* sb;
	Buf* pBuf;
	User& u = Kernel::Instance().GetUser();

	/* ��ȡSuperBlock������ڴ渱�� */
	sb = this->GetFS(dev);

	/* 
	 * ������д��̿����������ڱ���������������������
	 * ���ڲ������д��̿����������������������ͨ��
	 * ������������̵���Free()��Alloc()��ɵġ�
	 */
	while(sb->s_flock)
	{
		/* ����˯��ֱ����ø����ż��� */
		u.u_procp->Sleep((unsigned long)&sb->s_flock, ProcessManager::PINOD);
	}

	/* ��������ջ������ȡ���д��̿��� */
	blkno = sb->s_free[--sb->s_nfree];

	/* 
	 * ����ȡ���̿���Ϊ�㣬���ʾ�ѷ��価���еĿ��д��̿顣
	 * ���߷��䵽�Ŀ��д��̿��Ų����������̿�������(��BadBlock()���)��
	 * ����ζ�ŷ�����д��̿����ʧ�ܡ�
	 */
	if(0 == blkno )
	{
		sb->s_nfree = 0;
		Diagnose::Write("No Space On %d !\n", dev);
		u.u_error = User::ENOSPC;
		return NULL;
	}
	if( this->BadBlock(sb, dev, blkno) )
	{
		return NULL;
	}

	/* 
	 * ջ�ѿգ��·��䵽���д��̿��м�¼����һ����д��̿�ı��,
	 * ����һ����д��̿�ı�Ŷ���SuperBlock�Ŀ��д��̿�������s_free[100]�С�
	 */
	if(sb->s_nfree <= 0)
	{
		/* 
		 * �˴���������Ϊ����Ҫ���ж��̲������п��ܷ��������л���
		 * ����̨�Ľ��̿��ܶ�SuperBlock�Ŀ����̿���������ʣ��ᵼ�²�һ���ԡ�
		 */
		sb->s_flock++;

		/* ����ÿ��д��̿� */
		pBuf = this->m_BufferManager->Bread(dev, blkno);

		/* �Ӹô��̿��0�ֽڿ�ʼ��¼����ռ��4(s_nfree)+400(s_free[100])���ֽ� */
		int* p = (int *)pBuf->b_addr;

		/* ���ȶ��������̿���s_nfree */
		sb->s_nfree = *p++;

		/* ��ȡ�����к���λ�õ����ݣ�д�뵽SuperBlock�����̿�������s_free[100]�� */
		Utility::DWordCopy(p, sb->s_free, 100);

		/* ����ʹ����ϣ��ͷ��Ա㱻��������ʹ�� */
		this->m_BufferManager->Brelse(pBuf);

		/* ����Կ��д��̿������������������Ϊ�ȴ�����˯�ߵĽ��� */
		sb->s_flock = 0;
		Kernel::Instance().GetProcessManager().WakeUpAll((unsigned long)&sb->s_flock);
	}

	/* ��ͨ����³ɹ����䵽һ���д��̿� */
	pBuf = this->m_BufferManager->GetBlk(dev, blkno);	/* Ϊ�ô��̿����뻺�� */
	this->m_BufferManager->ClrBuf(pBuf);	/* ��ջ����е����� */
	sb->s_fmod = 1;	/* ����SuperBlock���޸ı�־ */

	return pBuf;
}

void FileSystem::Free(short dev, int blkno)
{
	SuperBlock* sb;
	Buf* pBuf;
	User& u = Kernel::Instance().GetUser();

	sb = this->GetFS(dev);

	/* 
	 * ��������SuperBlock���޸ı�־���Է�ֹ���ͷ�
	 * ���̿�Free()ִ�й����У���SuperBlock�ڴ渱��
	 * ���޸Ľ�������һ�룬�͸��µ�����SuperBlockȥ
	 */
	sb->s_fmod = 1;

	/* ������д��̿���������������˯�ߵȴ����� */
	while(sb->s_flock)
	{
		u.u_procp->Sleep((unsigned long)&sb->s_flock, ProcessManager::PINOD);
	}

	/* ����ͷŴ��̿�ĺϷ��� */
	if(this->BadBlock(sb, dev, blkno))
	{
		return;
	}

	/* 
	 * �����ǰϵͳ���Ѿ�û�п����̿飬
	 * �����ͷŵ���ϵͳ�е�1������̿�
	 */
	if(sb->s_nfree <= 0)
	{
		sb->s_nfree = 1;
		sb->s_free[0] = 0;	/* ʹ��0��ǿ����̿���������־ */
	}

	/* SuperBlock��ֱ�ӹ�����д��̿�ŵ�ջ���� */
	if(sb->s_nfree >= 100)
	{
		sb->s_flock++;

		/* 
		 * ʹ�õ�ǰFree()������Ҫ�ͷŵĴ��̿飬���ǰһ��100������
		 * ���̿��������
		 */
		pBuf = this->m_BufferManager->GetBlk(dev, blkno);	/* Ϊ��ǰ��Ҫ�ͷŵĴ��̿���仺�� */

		/* �Ӹô��̿��0�ֽڿ�ʼ��¼����ռ��4(s_nfree)+400(s_free[100])���ֽ� */
		int* p = (int *)pBuf->b_addr;
		
		/* ����д������̿��������˵�һ��Ϊ99�飬����ÿ�鶼��100�� */
		*p++ = sb->s_nfree;
		/* ��SuperBlock�Ŀ����̿�������s_free[100]д�뻺���к���λ�� */
		Utility::DWordCopy(sb->s_free, p, 100);

		sb->s_nfree = 0;
		/* ����ſ����̿�������ġ���ǰ�ͷ��̿顱д����̣���ʵ���˿����̿��¼�����̿�ŵ�Ŀ�� */
		this->m_BufferManager->Bwrite(pBuf);

		sb->s_flock = 0;
		Kernel::Instance().GetProcessManager().WakeUpAll((unsigned long)&sb->s_flock);
	}
	sb->s_free[sb->s_nfree++] = blkno;	/* SuperBlock�м�¼�µ�ǰ�ͷ��̿�� */
	sb->s_fmod = 1;
}

Mount* FileSystem::GetMount(Inode *pInode)
{
	/* ����ϵͳ��װ���� */
	for(int i = 0; i <= FileSystem::NMOUNT; i++)
	{
		Mount* pMount = &(this->m_Mount[i]);

		/* �ҵ��ڴ�Inode��Ӧ��Mountװ��� */
		if(pMount->m_inodep == pInode)
		{
			return pMount;
		}
	}
	return NULL;	/* ����ʧ�� */
}

bool FileSystem::BadBlock(SuperBlock *spb, short dev, int blkno)
{
	return 0;
}
