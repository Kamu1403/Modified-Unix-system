#include <stdio.h>
#include <sys.h>

int main1()
{
	int i;
	printf("%d %d \n", getpid(), getppid());
	for(i = 0; i < 3; ++i)
		if(fork()==0)
			printf("%d %d \n", getpid(), getppid());
	//sleep(2);
	return 1;
}
