#include <stdio.h>
#include <sys.h>

int main1(int argc, char* argv[])
{
	int i;
	for( i = 0; i < 3; i++ )
	{
		fork();
	}

	int pid = getpid();
	while(1)
	{
		sleep(pid%8+1);
		printf("PID :  %d\t", pid );
	}
	
	return 1;
}
