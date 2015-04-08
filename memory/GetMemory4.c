#include <stdio.h>
/*
正确运行, 打印
hello world
*/
char *GetMemory(char **p, int num)
{
	*p = (char *)malloc(num);
}
void Test(void)
{
	char *str = NULL;
	GetMemory(&str, 100);
	strcpy(str, "hello world");
	printf("%s\n", str);
}
int main(int argc, char *argv[])
{
	Test();
}
