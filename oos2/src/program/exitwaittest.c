#include <stdio.h>
#include <sys.h>

main1()
{
	int i,j;
	if(fork())
	{
		i = wait(&j);
		printf("It is parent process. \n");
		printf("The finished child process is %d. \n", i);
		printf("The exit status is %d. \n", j);
	}
	else
	{
		printf("It is child process. \n");
		exit(1);
	}
}
