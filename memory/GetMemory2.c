#include <stdio.h>
/*
答案 打印乱码
实际 不会乱码, 打印空的,没有任何字符
*/
char *GetMemory(void)
{
	char p[] = "hello world";
	return p;
}
void Test(void)
{
	char *str = NULL;
	str = GetMemory();
	printf("%s\n", str);
}
int main(int argc, char *argv[])
{
	Test();
}
