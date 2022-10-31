#include <stdio.h>
#include <sys.h>

int main1()
{
	int pri = getmypri();
	printf("here: My Process PRI is: %d\n", pri);
	return 1;
}
