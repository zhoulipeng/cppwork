#include <iostream>
using namespace std;

class Base
{
    public:
        void fun(int data)
        {
            cout<<"int"<<endl;
        }
};

class Derivd:public Base
{
    public:
        using Base::fun;// 如果注释using子类对象找不到父类版本的fun函数
        void fun(double data)
        {
            cout<<"double"<<endl;
        }
};

int main()
{
    Derivd d;
    d.fun(12);
    d.fun(12.0);
    d.Base::fun(12.0);
}
/*
[zhoulp@localhost abstract]$ ./DataHiding2.exe # 注释using
double
double
int
[zhoulp@localhost abstract]$ ./DataHiding2.exe # have using
int
double
int

 */
