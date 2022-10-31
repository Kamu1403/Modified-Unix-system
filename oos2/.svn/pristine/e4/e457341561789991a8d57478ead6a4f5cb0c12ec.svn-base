#include <stdio.h>
#include <stdlib.h>

int main1(int argc, char* argv[])
{
	int i;
	char line[100];
	
	for( i = 1; i < argc; i++)	//skip argv[0] = echo.exe
	{
		strcat(line, argv[i]);
		strcat(line, " ");	//space
	}
	printf("%s\n", line);
	
	return 1;
}
