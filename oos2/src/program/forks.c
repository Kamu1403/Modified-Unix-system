#include <stdio.h>
#include <sys.h>

/*   ��������ǶԵģ�����shell����������ʾ���ᴩ����ʾ�ڸ�Ӧ�õ����֮�С�֮��shell����������
 * ��ϵͳ��û����ʾ���ˡ��ı�������¡�
int main1(int argc, char* argv[])
{
	int i;
	for( i = 0; i < 3; i++ )
	{
		fork();
	}

	int pid = getpid();
	sleep(pid%8 + 1);
	printf("PID : %d\n", pid);
	
	return 1;
}
*/

//��������ʾ������Ӧ��ִ�����֮����֡���������ĳ������н���֮ǰ����Ļ�ϻ������������ʾ����
//�����Ƶ�ϵͳ�ǲ�Ӧ����ΪӦ�ó���ûд�ã��Ͷ�����������ʾ���ġ�
//shellӦ��robust!  HOW TO DO?���ȴ��������е����н���ȫ�������ٻ���shell��

int main1(int argc, char* argv[])
{
	int i;
	int kid,reCode;

	printf("PID : %d\n", getpid());
	for( i = 0; i < 3; i++ )
	{
		if(fork()==0)
			printf("PID : %d\n", getpid());
		else
			kid = wait(&reCode);
	}

	exit(0);
}
