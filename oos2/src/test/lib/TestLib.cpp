#include "TestLib.h"
#include "..\KernelInclude.h"
#include "..\TestInclude.h"

/* ��ϵͳ���ö�c.img���ļ����д�����д�����ݣ�����ȡ���顣 */
bool FileRWTest()
{
	Buf* pBuf;
	BufferManager& bufMgr = Kernel::Instance().GetBufferManager();
	User& u = Kernel::Instance().GetUser();

	int fd = -1;
	/* ϵͳ��ʼ��next()���Ѿ�������Ŀ¼"/"���˴������ļ�datafile */
	fd = lib_creat("/datafile", 0x1FF); /* mode = rwx rwx rwx  */
	
	int checksum = 0;
	int nblock = 7;//Inode::SMALL_FILE_BLOCK;	/* Ҫ���Ե��ļ�ռ���̿��� */
	
	/* ����С���ļ�д�� */
	for(int i = 0; i < nblock; i++ )
	{
		/* ��boot.bin��kernel.bin��Ϊdatafile������ */
		pBuf = bufMgr.Bread(DeviceManager::ROOTDEV, i);
		checksum += CheckSumBuffer(pBuf);
		int nbytes = lib_write(fd, (char *)pBuf->b_addr, Inode::BLOCK_SIZE);
		bufMgr.Brelse(pBuf);
		
		/* ʵ��д����ֽ��� */
		if ( nbytes != Inode::BLOCK_SIZE )
		{
			/* д��ʧ��!!! */
			Diagnose::Write("Lib_write failure! nbytes = [%d], u_error = %d\n", nbytes, u.u_error);
			return false;
		}
	}
	lib_close(fd);	/* close file handler */
	Diagnose::Write("Checksum = [%x]\n", checksum);
	Delay();Delay();

	/* u.u_error������г�����Ļ����������ĳ�����ȫ��ȷ���ں�Ҳ��������·�� **!!!!** */
	if ( User::NOERROR != u.u_error)
	{
		Diagnose::Write("u_error = %d\n", u.u_error);
		while(true);
	}

	/*****************************************/
	/*          ��ȡdatafile���ݲ�У��       */
	/*****************************************/

	fd = lib_open("/datafile", 0x1FF); /* mode = rwx rwx rwx  */
	int checksumRead = 0;
	/* ����һ�����棬���ڴ�Ŵ�datafile��ȡ�������� */
	//pBuf = bufMgr.Bread(DeviceManager::ROOTDEV, 1000);
	Buf b;
	pBuf = &b;
	char arr[Inode::BLOCK_SIZE];
	pBuf->b_addr = (unsigned char *)arr;
	
	for ( int i = 0; i < nblock; i++ )
	{
		int nRead = lib_read(fd, (char *)pBuf->b_addr, Inode::BLOCK_SIZE);
		checksumRead += CheckSumBuffer(pBuf);

		/* ʵ�ʶ�ȡ���ֽ��� */
		if ( nRead != Inode::BLOCK_SIZE )
		{
			/* ʧ��!!! */
			Diagnose::Write("Lib_read failure! nbytes = [%d], u_error = %d\n", nRead, u.u_error);
			return false;
		}
	}
	//bufMgr.Brelse(pBuf);
	lib_close(fd);
	Diagnose::Write("CheckSumRead = [%x]\n", checksumRead);

	if (checksum != checksumRead)
	{
		return false;
	}

	Diagnose::TraceOff();
	/* д��spb���ڴ�Inode���ӳ�д�̿� */
	Kernel::Instance().GetFileSystem().Update();
	Kernel::Instance().GetBufferManager().Bflush(DeviceManager::ROOTDEV);
	Diagnose::TraceOn();
	return true;
}

bool SelectTest()
{
	int pid = Kernel::Instance().GetProcessManager().NewProc();
	int priority = 40;
	User& u = Kernel::Instance().GetUser();

	if ( 0 == pid )
	{
		int ans = Kernel::Instance().GetProcessManager().NewProc();
		
		if ( ans == 0 )
		{
			while(true)
			{
				Diagnose::Write("~~~~~~~~~~ Process #0 ~~~~~~~~~~~~~\n");
				priority += 1;
				u.u_procp->p_pri = priority;
				Diagnose::Write("Priority #0 = [%d]\n", priority);
				Delay();
				Kernel::Instance().GetProcessManager().Swtch();
			}
		}
		else if ( 1 == ans )
		{
			while(true)
			{
				Diagnose::Write("~~~~~~~~~~ Process #2 ~~~~~~~~~~~~~\n");
				priority += 3;
				u.u_procp->p_pri = priority;
				Diagnose::Write("Priority #0 = [%d]\n", priority);
				Delay();
				Kernel::Instance().GetProcessManager().Swtch();
			}
		}
	}
	else if (1 == pid)
	{
		int ret = 0; //Kernel::Instance().GetProcessManager().NewProc();

		if ( 0 == ret )
		{
			while(true)
			{
				Diagnose::Write("~~~~~~~~~~ Process #1 ~~~~~~~~~~\n");
				priority += 2;
				u.u_procp->p_pri = priority;
				Diagnose::Write("Priority #0 = [%d]\n", priority);
				Delay();
				Kernel::Instance().GetProcessManager().Swtch();
			}
		}
		// else if ( 1 == ret )
		// {
			// while(true)
			// {
				// Diagnose::Write("################ Process #2 #############\n");
				// Delay();
				// Kernel::Instance().GetProcessManager().Swtch();
			// }
		// }
	}
	return true;
}

bool ForkTest()
{
	int priority = 40;
	User& u = Kernel::Instance().GetUser();
	u.u_procp->p_pri = priority;

	static int count = 0;

	/* ��fork��8�����̣�0# ~ 7# proc */
	for ( int i = 0; i < 2; i++ )
	{
		lib_fork();
	}

	count++;
	
	/* 1# ~ 7#ȫ��exit��0#����alive */
	if ( u.u_procp->p_pid != 0  )
	{
		//lib_exit(0x77);	/* u������д���������ϣ�����鿴u_MemoryDescriptor */
	}

	while(count < 4) ;	/* 8�����̵�ͬ���㣬ʱ���ж���Swtch() */
	
	Diagnose::Write("-------------->count = %d, pid = %d\n", count, u.u_procp->p_pid);
	if ( Time::lbolt >= Time::HZ - 1 )
	{
		Diagnose::Write("System Time: %d\n", Time::time);
	}
	while(true);

	/*************************************************/

	/* ����8#���� */
	int ans = lib_fork();
	if (ans == 0)
	{
		/* 
		 * ͨ��֮ǰ���̵�fork��exit���鿴�˴���������ʱu_MemoryDescriptor
		 * �Ƿ���ȷ������֤1# ~ 7#exit��Դ�Ƿ���ȷ�ͷ�
		 */
		lib_exit(0xEE);
	}
	if ( u.u_procp->p_pid == 0 )
	{
		u.u_procp->Sleep((unsigned long)u.u_procp, 0);
	}
	Diagnose::Write("==============>PID = %d, child id = %x\n", u.u_procp->p_pid, ans);
	
	return true;
}

bool ExitWaitTest()
{
	int priority = 40;
	User& u = Kernel::Instance().GetUser();
	u.u_procp->p_pri = priority;
	int pid = Kernel::Instance().GetProcessManager().NewProc();

	if ( 0 == pid )
	{
		while(true)
		{
			Diagnose::Write("~~~~~~~~~~ Process #0 ~~~~~~~~~~\n");
			u.u_procp->p_pri++;
			Diagnose::Write("Priority #0 = [%d]\n", u.u_procp->p_pri);
			Delay();
			Kernel::Instance().GetProcessManager().Swtch();
		}
	}
	else if (1 == pid)
	{
		int ret = Kernel::Instance().GetProcessManager().NewProc();
		
		if ( 0 == ret )
		{
			while(true)
			{
				Diagnose::Write("~~~~~~~~~~ Process #1 ~~~~~~~~~~\n");
				u.u_procp->p_pri++;
				Diagnose::Write("Priority #1 = [%d]\n", u.u_procp->p_pri);
				
				Diagnose::Write("Start lib_wait()...\n");
				Delay();
				int value = lib_wait(&ret);
				Diagnose::Write("lib_wait Return: [%d] status = [%x] addr = [%x] [%s] \n", value, ret, &ret, (-1 != value) ? "Child Exit" : "No Child");
				//Kernel::Instance().GetProcessManager().Swtch();
			}
		}
		else if ( 1 == ret )
		{
			int count = 2;
			while( --count )
			{
				Diagnose::Write("~~~~~~~~~~ Process #2 ~~~~~~~~~~\n");
				u.u_procp->p_pri++;
				Diagnose::Write("Priority #2 = [%d]\n", u.u_procp->p_pri);
				Delay();
				Kernel::Instance().GetProcessManager().Swtch();
			}

			Diagnose::Write("Process [%d] Exit!!\n", u.u_procp->p_pid);
			lib_exit(0x86 /* test ret Code */);
		}
	}
	return true; /* never reach here */
}


/********************************************/
/* 
ע�⣺����̬���޷���execv()�������ԣ���Ϊ
ջ��m_ES, m_DSΪ����̬��Selector����exec()
Fake���˳�����Ϊ�û�̬CS��SS���ᵼ���쳣��
*/
/********************************************/
bool ExecTest()
{
	char* argv[3];
	argv[0] = "/shell";
	char arr[10] = {'-', 'f', 'n', 'o', '-', 'p', 'w', 'd','\0'};
	argv[1] = arr;
	argv[2] = NULL;

	lib_execv("/Shell.exe", argv);
	return true;
}

bool CopyFileTest(char* srcFile, char* dstFile)
{
	int fsrc = -1;
	int fdst = -1;
	char buf[512];

	int fd = lib_open(dstFile, 0x1FF);
	if (fd > 0)
	{
		//dstFile EXIST!
		return true;
	}

	fsrc = lib_open(srcFile, 0x1FF);
	if (-1 == fsrc)
	{
		Diagnose::Write("Failed to Open src file.\n");
		return false;
	}
	fdst = lib_creat(dstFile, 0x1FF);
	if (-1 == fdst)
	{
		Diagnose::Write("Failed to Open dst file.\n");
		return false;
	}

	int rbytes = 0;
	int wbytes = 0;
	while (rbytes = lib_read(fsrc, buf, 512))
	{
		if (rbytes < 0)
		{
			Diagnose::Write("Read src file error...\n");
			return false;
		}
		else
		{
			wbytes = lib_write(fdst, buf, rbytes);
			if (rbytes != wbytes)
			{
				Diagnose::Write("Write dst file error...\n");
				return false;
			}
		}
	}
	lib_close(fsrc);
	lib_close(fdst);

	Kernel::Instance().GetFileSystem().Update();
	return true;
}

void PrepareExeFile()
{
	int exeBlkno = 19000;	/* Don't Change It */
	Buf* pBuf;
	BufferManager& bufMgr = Kernel::Instance().GetBufferManager();
	User& u = Kernel::Instance().GetUser();
	//char* exeName[10] = {"/shell.exe", "/peProgram.exe", "/cat", "/cp", "/ls", "/mkdir", "/rm"};
	char* exeName[10] = {"/peProgram.exe", "/Shell.exe"};

	pBuf = bufMgr.Bread(DeviceManager::ROOTDEV, exeBlkno);
	
	int count = 0;
	while(true)
	{
		int exeInfo[2];	/* exeInfo[0]�д��startBlkno��exeInfo[1]�д���ļ���С */

		Utility::DWordCopy((int *)(pBuf->b_addr + count * 2 * sizeof(int)), exeInfo, 2);
		
		if ( exeInfo[0] == 0 || exeInfo[1] == 0 )	/* �Ѿ��������exe�ļ�д�룬���� */
		{
			bufMgr.Brelse(pBuf);
			Diagnose::TraceOff();
			/* д��spb���ڴ�Inode���ӳ�д�̿� */
			Kernel::Instance().GetFileSystem().Update();
			Kernel::Instance().GetBufferManager().Bflush(DeviceManager::ROOTDEV);
			Diagnose::TraceOn();
			return;
		}

		int fd = -1;
		fd = lib_creat(exeName[count], 0x1FF);

		count++;

		int startBlkno = exeInfo[0];
		int size = exeInfo[1];	/* ��һ����512������ */
		Diagnose::Write("startBlkno = %d, size = %d\n", startBlkno, size);
		int i;
		Buf* pRead;
		for ( i = 0; i < size / Inode::BLOCK_SIZE; i++ )
		{
			pRead = bufMgr.Bread(DeviceManager::ROOTDEV, startBlkno + i);
			int nbytes = lib_write(fd, (char *)pRead->b_addr, Inode::BLOCK_SIZE);
			bufMgr.Brelse(pRead);
			/* ʵ��д����ֽ��� */
			if ( nbytes != Inode::BLOCK_SIZE )
			{
				/* д��ʧ��!!! */
				Diagnose::Write("Lib_write failure! nbytes = [%d], u_error = %d\n", nbytes, u.u_error);
				return ;
			}
		}
		if ( size % Inode::BLOCK_SIZE != 0 )
		{
			pRead = bufMgr.Bread(DeviceManager::ROOTDEV, startBlkno + i);
			int nbytes = lib_write(fd, (char *)pRead->b_addr, size % Inode::BLOCK_SIZE);
			bufMgr.Brelse(pRead);
			/* ʵ��д����ֽ��� */
			if ( nbytes != size % Inode::BLOCK_SIZE )
			{
				/* д��ʧ��!!! */
				Diagnose::Write("Lib_write failure! nbytes = [%d], u_error = %d\n", nbytes, u.u_error);
				return ;
			}
		}
		lib_close(fd);
	}
}

bool TTyQueueTest()
{
	TTy_Queue queue;
	char* string1 = "123456789012345";
	for ( int i = 0; i < 10; i++ )
	{
		queue.PutChar(string1[i]);
	}
	Delay();Delay();
	Diagnose::Write("queue Length = %d\n", queue.CharNum());
	for ( int i = 0; i < 2; i++ )
	{
		char ch = queue.GetChar();
		Diagnose::Write("char = %d, num = %d\n", ch, queue.CharNum());
	}
	while(true);

	return true;
}

bool ConsoleTest()
{
	CRT::ClearScreen();
	
	//User& u = Kernel::Instance().GetUser();

	//Diagnose::Write("pTTy addr = %x\n", u.u_procp->p_ttyp);
	//Diagnose::Write("t_state = %x, t_flags = %x\n", u.u_procp->p_ttyp->t_state, u.u_procp->p_ttyp->t_flags);
	//Delay();Delay();
	//while(true);

	char readin[80];
	Diagnose::TraceOff();

	while(true)
	{
		int nbytes = lib_read(0, readin, 80);
		if ( readin[nbytes - 1] == '\n' )
		{
			readin[nbytes - 1] = '\0';
		}
		//Diagnose::Write("Input %d: %s\n", nbytes, readin);

		lib_write(STDOUT, readin, nbytes);
	}

	while(true);
}

bool WorkingDirTest()
{
	User& u = Kernel::Instance().GetUser();
	char path[80];

	Diagnose::Write("u_curdir : %s\n", u.u_curdir);
	lib_chdir("/bin");
	Diagnose::Write("u_curdir : %s\n", u.u_curdir);

	lib_chdir("/noExist");
	Diagnose::Write("u_curdir : %s\n", u.u_curdir);
	
	lib_pwd(path);
	Diagnose::Write("PWD : %s\n", path);

	lib_chdir("/");
	lib_pwd(path);
	Diagnose::Write("PWD : %s\n", path);

	while(true);
}
