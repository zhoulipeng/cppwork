#include <stdio.h>
/**
 * http://www.cnblogs.com/jianxinzhou/p/3980114.html
 * 在C++中，POD是一个极其重要的概念。要理解POD类型，我们首先需要理解Aggregate类型。下文结合stackoverflow上的高票回答将对Aggregate类型做一个全面的解读。
对于Aggragates的定义
C++标准（C++ 03 8.5.1 §1）中的正式定义如下：
An aggregate is an array or a class (clause 9) with no user-declared constructors (12.1), no private or protected non-static data members (clause 11), no base classes (clause 10), and no virtual functions (10.3).
译文：一个Aggregate是一个数组或者一个没有用户声明构造函数，没有私有或保护类型的非静态数据成员，没有父类和虚函数的类型
*/
struct abc{
    abc(){
    }
    int a;
    int b;
    int c;
};

int main(int argc, char *argv[])
{
    //struct abc st = { 0 };//　如果struct abc定义构造函数则不能用该方法初始化
    struct abc st;
    char *p = reinterpret_cast<char*>(&st);
    printf("%hhx", *p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    printf("%hhx", *++p);
    return 0;
}
