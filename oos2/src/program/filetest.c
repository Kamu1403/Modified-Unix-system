#include <stdio.h>
#include <sys.h>
#include <file.h>

int main1()
{
	char data1[12] = "Hello World!"; //һ���ַ���
	char data2[12]; //������
	int fd = 0;     //�ļ����
	int count = 0;  //������

	/*
	 * ��/usrĿ¼�´��� Jessy�ļ�����Ϊ 8���Ƶ� 666 = 2���Ƶ� 110 110 110
	 * ����Ȩ��Ϊ�����û��ɶ���д
	 * �������ɹ����� fd����ļ���������򣬵õ� 0
	 */
	fd = creat("/usr/Jessy", 0666);

	if (fd > 0)
		printf("The file %d is created.\n", fd);  //��ӡ�ɹ���Ϣ
	else
		printf("The file can not be created.\n"); //��ӡʧ����Ϣ

	/*
	 * �� data1 ��ʼ�� 12 �ֽ�����д�� fdָ����ļ�
	 * ����ֵΪ�ɹ�д����ֽ���
	 */
	count = write(fd, data1, 12);

	/*
	 * �� 12 ���ֽ�ȫ��д�룬˵��д������ɹ�������д�����ʧ��
	 */
	if (count == 12)
		printf("The file is written %d characters .\n\n", count);
	else
		printf("The file can not be written successfully.\n\n");

	//�ر��ļ�
	close(fd);

	/*
	 * ��/usr/Jessy��ģʽΪֻ��
	 * 8���� 1 = 2���� 1
	 * ���򿪳ɹ����� fd ����ļ���������򣬵õ� 0
	 */
	fd = open("/usr/Jessy", 01);
	if (fd > 0)
		printf("The file %d is opened.\n", fd);  //��ӡ�ɹ���Ϣ
	else
		printf("The file can not be opened.\n"); //��ӡʧ����Ϣ


	seek(fd, 5, 0); //���ļ�ָ��ӿ�ͷλ�ú���5���ֽ�

	/*
	 * �� fd ָ����ļ���ʼλ�ã����� 12 �ֽڵ� data2
	 * ����ֵΪ�ɹ��������ֽ���
	 */
	count = read(fd, data2, 12);
	printf("%d characters are read from file %d: %s.\n", count, fd, data2); /* ��ӡ��ȡ�����ֽ������ļ���������������� */

	return 1;
}
