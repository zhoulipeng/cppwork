#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
free 只是释放的str指向的内存空间,它本身的值还是存在的.所以free之后，有一个好的习惯就是将str=NULL.
此时str指向空间的内存已被回收,如果输出语句之前还存在分配空间的操作的话,这段存储空间是可能被重新分配给其他变量的,
尽管这段程序确实是存在大大的问题（上面各位已经说得很清楚了），但是通常会打印出world来。
这是因为，进程中的内存管理一般不是由操作系统完成的，而是由库函数自己完成的。

　　当你malloc一块内存的时候，管理库向操作系统申请一块空间（可能会比你申请的大一些）
，然后在这块空间中记录一些管理信息（一般是在你申请的内存前面一点），并将可用内存的地址返回。
但是释放内存的时候，管理库通常都不会将内存还给操作系统，因此你是可以继续访问这块地址的。
*/
char *GetMemory(void)
{
	char p[] = "hello world";
	return p;
}
void Test(void)
{
	char *str = (char *) malloc(100);
	strcpy(str, "hello");
	free(str);
	if(str != NULL)
	{
		strcpy(str, "world");
		printf("%s\n", str);
	}
}
int main(int argc, char *argv[])
{
	Test();
}
