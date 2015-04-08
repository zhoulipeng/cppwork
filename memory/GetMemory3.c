#include <stdio.h>
/*
答案 打印乱码
实际 不会乱码, 打印空的,没有任何字符
*/
char *GetMemory(char *p)
{
	p = (char *)malloc(100);
	return p;
}
void Test(void)
{
	char *str = NULL;
	GetMemory(str);
	strcpy(str, "hello world");
	printf("%s\n", str);
}
int main(int argc, char *argv[])
{
	Test();
}
