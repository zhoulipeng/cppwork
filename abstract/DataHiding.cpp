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
        using Base::fun; // 注释调这行报错，编译器找不到, 说明编译器只会搜索子类，不会搜索父类
        void fun(double data, int o) // 会隐藏调父类的fun
        {
            cout<<"double"<<endl;
        }
};

int main()
{
    Derivd d;
    d.fun(12); // call1
    d.fun(12.0); // call2
    d.Base::fun(12.0);
}
