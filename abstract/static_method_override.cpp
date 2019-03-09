#include <iostream>
using namespace std;

template <class T>
struct base {
    static void foo()
    {
        T::bar();
    }
};

struct derived : public base<derived> {
    static void bar()
    {
        std::cout << "derived" << std::endl;
    }
};
///////////////////////////////////////////////
class Base {
public:
    static int a;
    static void Fun()
    {
        cout << "静态成员函数可以被继承!Base" << endl;
    }
};

int Base::a = 1000;

class Derived : public Base {
//public:
//    static void Fun()
//    {
//        cout << "静态成员函数可以被overide" << endl;
//    }
};

int main()
{
    derived::foo();
    cout << "静态数据成员可以被继承!  " << Derived::a << endl;
    // 如果没有DerivedFun 会执行Base::Fun
    Derived::Fun();
    Base::Fun();
}
