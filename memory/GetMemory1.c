#include <stdio.h>
/*
打印 hello world
*/
char *GetMemory(void)
{
	char *p = "hello world";
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
