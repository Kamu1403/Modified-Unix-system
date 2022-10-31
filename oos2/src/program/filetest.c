#include <stdio.h>
#include <sys.h>
#include <file.h>

int main1()
{
	char data1[12] = "Hello World!"; //一个字符串
	char data2[12]; //缓冲区
	int fd = 0;     //文件句柄
	int count = 0;  //计数器

	/*
	 * 在/usr目录下创建 Jessy文件，因为 8进制的 666 = 2进制的 110 110 110
	 * 所以权限为所有用户可读可写
	 * 若创建成功，则 fd获得文件句柄；否则，得到 0
	 */
	fd = creat("/usr/Jessy", 0666);

	if (fd > 0)
		printf("The file %d is created.\n", fd);  //打印成功信息
	else
		printf("The file can not be created.\n"); //打印失败信息

	/*
	 * 将 data1 开始的 12 字节数据写入 fd指向的文件
	 * 返回值为成功写入的字节数
	 */
	count = write(fd, data1, 12);

	/*
	 * 若 12 个字节全部写入，说明写入操作成功；否则，写入操作失败
	 */
	if (count == 12)
		printf("The file is written %d characters .\n\n", count);
	else
		printf("The file can not be written successfully.\n\n");

	//关闭文件
	close(fd);

	/*
	 * 打开/usr/Jessy，模式为只读
	 * 8进制 1 = 2进制 1
	 * 若打开成功，则 fd 获得文件句柄；否则，得到 0
	 */
	fd = open("/usr/Jessy", 01);
	if (fd > 0)
		printf("The file %d is opened.\n", fd);  //打印成功信息
	else
		printf("The file can not be opened.\n"); //打印失败信息


	seek(fd, 5, 0); //将文件指针从开头位置后移5个字节

	/*
	 * 从 fd 指向的文件起始位置，读入 12 字节到 data2
	 * 返回值为成功读到的字节数
	 */
	count = read(fd, data2, 12);
	printf("%d characters are read from file %d: %s.\n", count, fd, data2); /* 打印读取到的字节数、文件句柄、读到的内容 */

	return 1;
}
