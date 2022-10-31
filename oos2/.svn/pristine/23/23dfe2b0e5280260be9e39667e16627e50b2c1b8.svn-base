#ifndef USER_H
#define USER_H

#include "MemoryDescriptor.h"
#include "Process.h"
#include "File.h"
#include "INode.h"
#include "FileManager.h"

/*
 * @comment 该类与Unixv6中 struct user结构对应，因此只改变
 * 类名，不修改成员结构名字，关于数据类型的对应关系如下:
 */
class User
{
public:
	static const int EAX = 0;	/* u.u_ar0[EAX]；访问现场保护区中EAX寄存器的偏移量 */
	
	/* u_error's Error Code */
	enum ErrorCode
	{
		NOERROR	= 0,
		EPERM	= 1,
		ENOENT	= 2,
		ESRCH	= 3,
		EINTR	= 4,
		EIO		= 5,
		ENXIO	= 6,
		E2BIG	= 7,
		ENOEXEC	= 8,
		EBADF	= 9,
		ECHILD	= 10,
		EAGAIN	= 11,
		ENOMEM	= 12,
		EACCES	= 13,
		ENOTBLK	= 15,
		EBUSY 	= 16,
		EEXIST	= 17,
		EXDEV	= 18,
		ENODEV	= 19,
		ENOTDIR	= 20,
		EISDIR	= 21,
		EINVAL	= 22,
		ENFILE	= 23,
		EMFILE	= 24,
		ENOTTY	= 25,
		ETXTBSY	= 26,
		EFBIG	= 27,
		ENOSPC	= 28,
		ESPIPE	= 29,
		EROFS	= 30,
		EMLINK	= 31,
		EPIPE	= 32,
		ENOSYS	= 100,
		EFAULT	= 106
	};

	static const int NSIG = 25;	/* 信号个数 */

	/* p_sig中接受到的信号定义 */
	static const int SIGNUL = 0;	/* No Signal Received */
	static const int SIGDIV = 1;	/* Divide Error */
	static const int SIGDEX = 2;	/* Debug Exceptions */
	static const int SIGBKP	= 3;	/* Breakpoint */
	static const int SIGOVF	= 4;	/* Overflow */
	static const int SIGBOC	= 5;	/* Bounds Check */
	static const int SIGIVP	= 6;	/* Invalid Opcode */
	static const int SIGCNA	= 7;	/* Co-processor Not Available */
	static const int SIGDFT	= 8;	/* Double Fault */
	static const int SIGCSO	= 9;	/* Co-processor Segment Overrun */
	static const int SIGINT	= 10;	/* Invalid TSS */
	static const int SIGSNP	= 11;	/* Segment Not Present */
	static const int SIGSTE	= 12;	/* Stack Exception */
	static const int SIGGPE	= 13;	/* General Protection Exception */
	static const int SIGPPV	= 14;	/* Page protection violation */
	static const int SIGNEM	= 15;	/* Not enough physical memory */
	static const int SIGAUM	= 16;	/* Access unallocated memory */
	static const int SIGCER	= 17;	/* Co-processor Error */
	static const int SIGALC = 18;	/* Alignment Check Exception */
	static const int SIGMCK	= 19;	/* Machine Check Exception */
	static const int SIGFPE = 20;	/* SIMD Float Point Exception */
	static const int SIGKIL	= 21;	/* kill */
	static const int SIGBRK	= 22;	/* Ctrl+Break */
	static const int SIGPIPE = 23;	/* end of pipe */

public:
	unsigned long u_rsav[2];	/* 用于保存esp与ebp指针 */
	unsigned long u_ssav[2];	/* 用于对esp和ebp指针的二次保护 */
	Process* u_procp;			/* 指向该u结构对应的Process结构 */
	/* 新添加变量，用于替代原有的变量
	 * int u_uisa[16]
	 * int u_uisd[16]
	 * u_tsize
	 * u_dsize
	 * u_ssize
	 */
	MemoryDescriptor u_MemoryDescriptor;

	/* 系统调用相关成员 */
	unsigned int	*u_ar0;		/* 指向核心栈现场保护区中EAX寄存器
								存放的栈单元，本字段存放该栈单元的地址。
								在V6中r0存放系统调用的返回值给用户程序，
								x86平台上使用EAX存放返回值，替代u.u_ar0[R0] */

	int u_arg[5];				/* 存放当前系统调用参数 */
	char* u_dirp;				/* 系统调用参数(一般用于Pathname)的指针 */

	/* 时间相关成员 */
	int u_utime;		/* 进程用户态时间 */
	int u_stime;		/* 进程核心态时间 */
	int u_cutime;		/* 子进程用户态时间总和 */
	int u_cstime;		/* 子进程核心态时间总和 */
	
	/* 信号处理相关成员 */
	unsigned long u_signal[NSIG];	/* 信号处理表 */
	unsigned long u_qsav[2];		/* 用于接收到信号时直接从Sleep()函数跳回至Trap() */
	bool u_intflg;		/* 系统调用期间是否受到信号打断 */
	
	/* 文件系统相关成员 */
	Inode* u_cdir;		/* 指向当前目录的Inode指针 */
	Inode* u_pdir;		/* 指向父目录的Inode指针 */

	DirectoryEntry u_dent;					/* 当前目录的目录项 */
	char u_dbuf[DirectoryEntry::DIRSIZ];	/* 当前路径分量 */
	char u_curdir[128];						/* 当前工作目录完整路径 */

	ErrorCode u_error;			/* 存放错误码 */
	int u_segflg;				/* 表明I/O针对用户或系统空间 */

	/* 进程的用户标识 */
	short u_uid;		/* 有效用户ID */
	short u_gid;		/* 有效组ID */
	short u_ruid;		/* 真实用户ID */
	short u_rgid;		/* 真实组ID */
	
	/* 文件系统相关成员 */
	OpenFiles u_ofiles;		/* 进程打开文件描述符表对象 */

	/* 文件I/O操作 */
	IOParameter u_IOParam;	/* 记录当前读、写文件的偏移量，用户目标区域和剩余字节数参数 */

	/* Member Functions */
public:
	/* 根据系统调用参数uid设置有效用户ID，真实用户ID，进程用户ID(p_uid) */
	void Setuid();

	/* 获取用户ID，低16比特为真实用户ID(u_ruid)，高16比特为有效用户ID(u_uid) */
	void Getuid();

	/* 根据系统调用参数gid设置有效组ID，真实组ID */
	void Setgid();

	/* 获取组ID, 低16比特为真实组ID(u_rgid)，高16比特为有效组ID(u_gid) */
	void Getgid();

	/* 获取当前用户工作目录 */
	void Pwd();

	/* 检查当前用户是否是超级用户 */
	bool SUser();
};

#endif

