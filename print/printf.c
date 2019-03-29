#include <stdio.h>
#include <unistd.h>
/*
所有的转义字符和所对应的意义：
转义字符	意义	ASCII码值（十进制）
\a		响铃(BEL)				007
\b		退格(BS) ，将当前位置移到前一列		008
\f		换页(FF)，将当前位置移到下页开头	012
\n		换行(LF) ，将当前位置移到下一行开头	010
\r		回车(CR) ，将当前位置移到本行开头	013
\t		水平制表(HT) （跳到下一个TAB位置）	009
\v		垂直制表(VT)				011
\\		代表一个反斜线字符''\'			092
\'		代表一个单引号（撇号）字符		039
\"		代表一个双引号字符			034
\0		空字符(NULL)				000
\ddd		1到3位八进制数所代表的任意字符		三位八进制
\xhh		1到2位十六进制所代表的任意字符		二位十六进制
二、%#x和%x的区别
三、 打印指针%p
四、 实现类似github下载 刷新打印百分比，但不换行，需要使用 \r 与fflush 
函数配合使用, 因为linux控制台使用行缓冲策略，\n会触发行缓冲实践显示到屏幕
因此\r 需要与fflush配合使用
Rceiving objects:  12% (3867/31189), 40.64 MiB | 41 KiB/s
五、为什么char 与 unsigned char变量 char x = 0xff; unsigned char y = 0xff;
用 %02hx 打印的结果会不同
六、刘永刚在群里发的问题，待测试
 snprintf  %lld 格式化int64_t  好坑爹。 编译一点没问题，运行时超过4字节大小的，格式结果变为负数 越界了。
 正确姿势 %I64d
七、
http://blog.csdn.net/yanlin1989812/article/details/22063669 
#if defined(_MSC_VER)  
    #define JL_SIZE_T_SPECIFIER "%Iu"  
    #define JL_SSIZE_T_SPECIFIER "%Id"  
    #define JL_PTRDIFF_T_SPECIFIER "%Id"  
#elif defined(__GNUC__)  
    #define JL_SIZE_T_SPECIFIER "%zu"  
    #define JL_SSIZE_T_SPECIFIER "%zd"  
    #define JL_PTRDIFF_T_SPECIFIER "%zd"  
#else // TODO figure out which to use.  
    #if NUMBITS == 32  
        #define JL_SIZE_T_SPECIFIER something_unsigned  
        #define JL_SSIZE_T_SPECIFIER something_signed  
        #define JL_PTRDIFF_T_SPECIFIER something_signed  
    #else  
        #define JL_SIZE_T_SPECIFIER something_bigger_unsigned  
        #define JL_SSIZE_T_SPECIFIER something_bigger_signed  
        #define JL_PTRDIFF_T_SPECIFIER something-bigger_signed  
    #endif  
#endif  
*/
int main(int argc, char *argv[])
{
    char x = 0xff;
    unsigned char y = 0xff;
    printf("x = %02hx, %02hx\n", x, y);
    printf("x = %02hx, %02hx\n", x & 0xff, y);
    printf("x = %hhx, %hhx\n", x, y);
	char c = '\a'; // ring char
	printf("char is %%c\t\t:value=%c\n", '\a');
	long unsigned int lu = 999999999;
	printf("long unsigned int %%lu: value=%lu\n", lu);
	int percent = 0;
	do{
		printf("Rceiving %d%%\r", percent);
		//fprintf(stdout, "decode one audio frame!\b\r");
		fflush(stdout);
		percent++;
		sleep(1);
	}while(percent < 100);
	const int NUM  = 50;//任务完成总量  
      
}
