// http://blog.csdn.net/chenlycly/article/details/42061009 #pragma pack 字节对齐预编译指令说明 
// http://blog.csdn.net/liukun321/article/details/6974282  # 结构体在内存中的对齐规则
/*
   结构体元素自然对齐原则 ：
   原则一：结构体中元素是按照定义顺序一个一个放到内存中去的，但并不是紧密排列的。
从结构体存储的首地址开始，每一个元素放置到内存中时，它都会认为内存是以它自己的大
小来划分的，因此元素放置的位置一定会在自己宽度的整数倍上开始（以结构体变量首地址
为0计算）。
  原则二：在经过第一原则分析后，检查计算出的存储单元是否为所有元素中最宽的元素的
长度的整数倍，是，则结束；若不是，则补齐为它的整数倍。
  原则三: 复杂类型-- 结构体包含结构体
   4，复杂类型（如结构）整体的对齐是按照结构体中长度最大的数据成员和
#pragma pack指定值之间较小的那个值进行；这样在成员是复杂类型时，可以最小化长度；
 例八：

                                      struct X

                                     {

                                          char a;

                                          int b;

                                          double c;

                                      };

                                      struct Y

                                      {

                                           char a;

                                           X b;

                                       };

    经测试，可知sizeof(X)为16，sizeof(Y)为24。即计算Y的存储长度时，在存放第二个元素b时的初始位置是在
double型的长度8的整数倍处，而非16的整数倍处，即系统为b所分配的存储空间是第8~23个字节。

    如果将Y的两个元素char型的a和X型的b调换定义顺序，则系统为b分配的存储位置是第0~15个字节，为a分配的
是第16个字节，加起来一共17个字节，不是最长基本类型double所占宽度8的整数倍，因此要补齐到8的整数倍，
即24。测试后可得sizeof(Y)的值为24。



    补充数组 ：复杂类型--  数组也属于复杂类型 按中长度最大的数据成员，因为数组的元素都一样， 因此，结构体中的数组安数组基本类型对齐
*/

#include <iostream>
using namespace std;
struct X
{
    char a;
    int b;
    double c;
}S1;

int main()
{
    cout << sizeof(S1) << endl;
    cout << sizeof(S1.a) << endl;
    cout << sizeof(S1.b) << endl;
    cout << sizeof(S1.c) << endl;
}
