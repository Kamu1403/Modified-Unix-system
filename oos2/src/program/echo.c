#include <stdio.h>
#include <stdlib.h>

int main1(int argc, char* argv[])
{
	int i;
	char line[100];
	
	for( i = 1; i < argc; i++)	//skip argv[0] = echo.exe
	{
		//strcat(line, argv[i]);
		printf("%s ", argv[i]);
		//strcat(line, " ");	//space
	}
	
	printf("\n");
	//printf("HEAD %s size of line=%d\n", line, sizeof(line));

	//printf("%s\n", line);

	//return 1;
	exit(0);
}
