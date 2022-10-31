#include <stdio.h>
#include <sys.h>

void SIGBRK_Handler()
{
	printf("Child: \nThis is #SIGBRK signal handler!\n");
}

void divide_err_handler()
{
	printf("Divide Error handler...\n");
}

int main1(int argc, char* argv[])
{
	// signal(1, divide_err_handler);
	// int err_val = 10 / 0;		/* 测试用户态Exception向产生异常的进程发送信号，并终止该进程的效果 */
	
	signal(SIGBRK, SIGBRK_Handler);
	int pid = fork();
	if( pid == 0 )
	{
		sleep(50);
		
		printf("Signal from parent received.\n");
		
		exit(0x88);
	}
	else
	{
		sleep(1);
		printf("Parent: Send #SIGBRK to child.\n");
		int ans = kill(pid, SIGBRK);
		if (ans == -1)
			printf("#SIGBRK Sent failed.\n");
		
		int status;
		ans = wait(&status);
		printf("Parent : child process exit [%d],[%d]\n", status, ans);
	}
	
	return 1;
}
