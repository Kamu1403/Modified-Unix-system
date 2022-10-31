#ifndef SYS_H
#define SYS_H

#include <time.h>

	/* p_sig�н��ܵ����źŶ��� */
	#define SIGNUL  0	/* No Signal Received */
	#define SIGHUP  1	/* Hangup (kill controlling terminal) */
	#define SIGINT  2    /* Interrupt from keyboard */
	#define SIGQUIT  3	/* Quit from keyboard */
	#define SIGILL  4	/* Illegal instrution */
	#define SIGTRAP  5	/* Trace trap */
	#define SIGABRT  6	/* use abort() API */
	#define SIGBUS  7	/* Bus error */
	#define SIGFPE  8	/* Floating point exception */
	#define SIGKILL  9	/* Kill(can't be caught or ignored) */
	#define SIGUSR1  10	/* User defined signal 1 */
	#define SIGSEGV  11	/* Invalid memory segment access */
	#define SIGUSR2  12	/* User defined signal 2 */
	#define SIGPIPE  13	/* Write on a pipe with no reader, Broken pipe */
	#define SIGALRM  14	/* Alarm clock */
	#define SIGTERM  15	/* Termination */
	#define SIGSTKFLT  16 /* Stack fault */
	#define SIGCHLD  17 /* Child process has stopped or exited, changed */
	#define SIGCONT  18 /* Continue executing, if stopped */
	#define SIGSTOP  19 /* Stop executing */
	#define SIGTSTP  20 /* Terminal stop signal */
	#define SIGTTIN  21 /* Background process trying to read, from TTY */
	#define SIGTTOU  22 /* Background process trying to write, to TTY */
	#define SIGURG  23  /* Urgent condition on socket */
	#define SIGXCPU  24 /* CPU limit exceeded */
	#define SIGXFSZ  25 /* File size limit exceeded */
	#define SIGVTALRM  26 /* Virtual alarm clock */
	#define SIGPROF  27 /* Profiling alarm clock */
	#define SIGWINCH  28 /* Window size change */
	#define SIGIO  29 /* I/O now possible */
	#define SIGPWR  30 /* Power failure restart */
	#define SIGSYS  31 /* invalid sys call */

int execv(char* pathname, char* argv[]);

int fork();

int wait(int* status);

int exit(int status);

int signal(int signal, void (*func)());

int kill(int pid, int signal);

int sleep(unsigned int seconds);

int brk(void * newEndDataAddr);

int sbrk(int increment);

int syncFileSystem();

int getPath(char* path);

int getpid();

unsigned int getgid();

unsigned int getuid();

int setgid(short gid);

int setuid(short uid);

int gettime(struct tms* ptms);   /* ��ϵͳʱ�� */

/* ��ȡ�����û�̬������̬CPUʱ��Ƭ�� */
int times(struct tms* ptms);

/* ��ȡϵͳ�����л����� */
int getswtch();

/* ������Ļ�ײ���lines�����������Ϣ */
int trace(int lines);



#endif
