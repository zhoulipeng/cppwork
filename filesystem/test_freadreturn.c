/*
 *  size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
 *  测试fread(buffer, 1024, 1, file) 文件数据小于 size 的时候返回值
 *  测试结果：虽然返回0 但是文件内容已经读取完毕
 */

#include <stdio.h>

int main(int argc, char *argv[])
{
	char buf[1024] = { 0 };
	FILE *file = fopen("test_freadreturn.c", "r");
	if(file)
	{
		size_t ret = fread(buf, 1024, 1, file);
		printf("fread ret = %d, buf = \n{\n%s\n}\n", ret, buf);
		ret = fread(buf, 400, 1, file);
                printf("fread ret = %d\n", ret);
	}
	return 0;
}
