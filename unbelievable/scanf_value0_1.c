/* 
 * scanf 影响-=取变量值的问题
 * 本例中 去掉scanf那一行， 直接赋值a=2 printf 打印a = -4
 *
 * 这个问题在gcc下面会导致比较奇葩的数值不一致问题，但是clang编译没有这个问题,
 * 并且会打印警告信息
 * scanf_value0_1.c:7:16: warning: unsequenced modification and access to 'a' [-Wunsequenced]
 */
#include <stdio.h>
#include <stdlib.h>
int main()
{
    int a = 2;
    a = a - (a = a + 4);//这种写法在gcc中会导致异常的问题
    printf("a = %d\n", a);
    return 0;
}
