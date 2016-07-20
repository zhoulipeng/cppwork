/*
 * http://blog.chinaunix.net/uid-20671208-id-3013780.html
 * 分析：
 *
 * 1，  使用颜色打印调试信息：
 *
 *  
 *
 *      printf("\033[46;31m[%s:%d]\033[0m "#fmt" errno=%d, %m\r\n", __func__, __LINE__, ##args, errno, errno);
 *
 *      上面printf时在Linux 命令行下打印出带颜色的字体，方便一眼区分不同种类的调试信息，只需要加上一些颜色代码，例如：这里的46代表底色, 31代表字体的颜色。
 *
 *      使用ascii code 是对颜色调用的始末格式如下：
 *
 *       
 *
 *      \033[ ; m …… \033[0m
 *
 *      后面哪个 ”\033[0m” 是对前面哪个颜色载入的结束，恢复到终端原来的背景色和字体色，可以把后面哪个修改成如下试试：
 *
 *            
 *
 *      #define DEBUG_ERR(fmt, args...) printf("\033[46;31m[%s:%d]\033[40;37m "#fmt" errno=%d, %m\r\n", __func__, __LINE__, ##args, errno, errno);
 *
 *      下面列出 ascii code 的颜色值：
 *
 *                   字背景颜色范围:40----49                            字颜色:30-----------39
 *
 *                         40:黑                                                   30:黑
 *
 *                         41:深红                                                 31:红
 *
 *                         42:绿                                                   32:绿
 *
 *                         43:黄色                                                 33:黄
 *
 *                         44:蓝色                                                 34:蓝色
 *
 *                         45:紫色                                                 35:紫色
 *
 *                         46:深绿                                                 36:深绿
 *
 *                         47:白色                                                 37:白色
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifndef __USE_DEBUG
#define __USE_DEBUG

#define USE_DEBUG
#ifdef USE_DEBUG
#define DEBUG_LINE() printf("[%s:%s] line=%d\r\n",__FILE__, __func__, __LINE__)
#define DEBUG_ERR(fmt, args...) printf("\033[46;31m[%s:%d]\033[0m "#fmt" errno=%d, %m\r\n", __func__, __LINE__, ##args, errno, errno)
#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%d]\033[0m "#fmt"\r\n", __func__, __LINE__, ##args)
#else
#define DEBUG_LINE()
#define DEBUG_ERR(fmt, ...)
#define DEBUG_INFO(fmt, ...)
#endif

#endif

void func()
{
        DEBUG_LINE();
        DEBUG_INFO("Garfield test DEBUG_INFO() d: %d ; s: %s", 1 , __FUNCTION__);
        DEBUG_ERR("Garfield test DEBUG_ERR() d: %d ; s: %s", 2 , __FUNCTION__);
}

int main(int argc,char **argv)
{
        func();

        return 0;
}
