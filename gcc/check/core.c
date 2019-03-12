#include <stdlib.h>
#include <stdio.h>

struct elem{
    long a;
    double b;
    char *c;
};

int main()
{
    char *p = malloc(sizeof(char) * 10);
    if (p == NULL) {//error p的内存正常情况下没有释放　
        return 0;
    }

    struct elem *e = malloc(sizeof(struct elem));
    if (e == NULL) {
        free(p);
        return 0;
    }

    e->a = 10;
    e->b = 10.10;
    e->c = p;

    double *xx = &e->b;
    printf("%f\n", *xx);
    struct elem *e2 = e;
    free(e);
    // free(e2);
    printf("%f\n", *xx); // error 已经释放，还是使用的内存错误，野指针

    return 0;
}
/*
---------------------example 1------------------------------
https://www.cnblogs.com/cobbliu/p/4433341.html

yb@darkstar:~/zhoulp/github/cppwork/gcc/check$ gcc  -g  -fsanitize=leak -fno-omit-frame-pointer -o core.exe core.c
yb@darkstar:~/zhoulp/github/cppwork/gcc/check$ ./core.exe 
10.100000
10.100000

=================================================================
==31126==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 10 byte(s) in 1 object(s) allocated from:
    #0 0x7f1701c435a6 in __interceptor_malloc (/usr/lib64/liblsan.so.0+0xd5a6)
    #1 0x4011cb in main /home/yb/zhoulp/github/cppwork/gcc/check/core.c:12
    #2 0x7f170186dba6 in __libc_start_main (/lib64/libc.so.6+0x21ba6)

SUMMARY: LeakSanitizer: 10 byte(s) leaked in 1 allocation(s).


---------------------2------------------------------
 -fsamitize=address
gcc  -g  -fsanitize=address -fno-omit-frame-pointer -o core.exe core.c
检查内存越界,野指针等好像不能同时开启，个人建议平常开启内存泄露检查，明显有崩溃等错误时开启内存越界检查

*/
