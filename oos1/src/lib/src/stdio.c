#include <stdio.h>
#include "file.h"
#include "valist.h"

char *g_STDIO_str = "Unix V6++";
int g_STDIO_data = 21;
char g_STDIO_bss[10];

extern int _sprintf(char* buffer, char* fmt, va pva);

void printf(char* fmt, ... )
{
	char buffer[1024];
	va pva = getvahead(fmt);
	int count = _sprintf(buffer, fmt, pva);	
	write(STDOUT, buffer, count);
}

void gets(char * s)
{
	int n = read(STDIN, s, 1024);
	/* ��/n���磬������Ҫɾȥ/n */
	if ( s[n - 1] == '\n' ) s[n - 1] = 0;
}
