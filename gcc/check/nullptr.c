#include <stdio.h>

int main(int argc, char *argv[])
{
    int *p = NULL;
    *p = 10;
    printf("%d", *p);
    return 0;
}
/*
gcc4.8及以上版本支持地址越界，野指针检查，只需要在编译时使用-fsanitize=address选项即可，
当运行程序时如果访问非本程序申请的地址就会报错。


yb@darkstar:~/zhoulp/github/cppwork/gcc$  gcc -g -fsanitize=address -o nullpoint nullptr.c
yb@darkstar:~/zhoulp/github/cppwork/gcc$ ./nullpoint
ASAN:DEADLYSIGNAL
=================================================================
==13620==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 
(pc 0x00000040123c bp 0x7ffc3e7605a0 sp 0x7ffc3e760580 T0)
==13620==The signal is caused by a WRITE memory access.
==13620==Hint: address points to the zero page.
    #0 0x40123b in main /home/yb/zhoulp/github/cppwork/gcc/nullptr.c:6
    #1 0x7f72b4c74ba6 in __libc_start_main (/lib64/libc.so.6+0x21ba6)
    #2 0x4010c9 in _start (/home/yb/zhoulp/github/cppwork/gcc/nullpoint+0x4010c9)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV /home/yb/zhoulp/github/cppwork/gcc/nullptr.c:6 in main
==13620==ABORTING

yb@darkstar:~/zhoulp/github/cppwork/gcc$  gcc -g  -o nullpoint nullptr.c
yb@darkstar:~/zhoulp/github/cppwork/gcc$ ./nullpoint
段错误


从上面的例子程序可以看到
*/
