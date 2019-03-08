#include <iostream>

// 原文：https://blog.csdn.net/qq_29344757/article/details/78880376 
/* 
 * c++类中成员有3种访问属性：public、private和protected，派生类对基类的继承方式同样是这3种。
 * c++类对一个继承而来的成员的访问权限受两个因素影响：
  (1) 该成员在基类中的访问权限说明符
  (2) 派生类的继承方式
  需要注意，上述是站在一个派生类的使用者(包括继承该派生类的新派生类)而言的。所站在派生类的实现者而言，
  他对继承而来的成员的访问权限只是上述的第一点,见下面三个继承类，报错的位置都一样，完全取决于基类的定义
  ，即只和该成员在基类中的访问权限说明符决定。
*/
using namespace std;
class Base
{
public:
    int pub_mem;
protected:
    int pro_mem;
private:
    int pri_mem;
};

class Pub_Derv : public Base
{
public:
    void out_put_base_data()
    {
        cout << pub_mem << endl;    //正确，public成员是公有的
        cout << pro_mem << endl;    //正确，基类中protected成员可以被派生类访问
        //cout << pri_mem << endl;    //错误，private成员只能在所在类内使用
    }
};

//class Pro_Derv : protected Base
//{
//public:
//    void out_put_base_data()
//    {
//        cout << pub_mem << endl;    //正确，public成员是公有的
//        cout << pro_mem << endl;    //正确，基类中protected成员可以被派生类访问
//        cout << pri_mem << endl;    //错误，private成员只能在所在类内使用
//    }
//};
//
//class Pri_Derv : privater Base
//{
//public:
//    void out_put_base_data()
//    {
//        cout << pub_mem << endl;    //正确，public成员是公有的
//        cout << pro_mem << endl;    //正确，基类中protected成员可以被派生类访问
//        cout << pri_mem << endl;    //错误，private成员只能在所在类内使用
//    }
//};

/*
 * 
上面的代码执行结果可见，派生类的继承说明符的目的是控制派生类视同这对于基类成员的访问权限。
若继承方式是public的，则成员将遵循其原有的在基类中的访问说明符；若继承方式是protected的，
则基类的所有public成员都是protected的，此时只能是派生类的成员和友元可以访问这些些继承而来
的public成员。若继承是private的，则在派生类中所有public、protected基类成员都是private的，
同理，只有派生类的成员函数和友元才能访问这些继承而来的原先是public、protected的成员。
另外注意，派生类的继承方式还会控制继承自派生类的新类的访问权限，如下代码：
*/


class Dev_from_Pub : public Pub_Derv
{
public:
    int use_base() { return pro_mem; }     //正确，Base::pro_mem在Pub_Derv仍旧是protected的
};

class Dev_from_Pri : public Pri_Derv
{
public:
    int use_base() { return 0; /* pro_mem;*/ }     // 错误，Base::pro_mem在Pri_Derv是privater的
};

int main(void)
{
    Pub_Derv d1;        //Pub_Derv是public方式继承Base的，所以在派生类中所有基类成员保持其原访问属性
    //使用者要考虑基类定义与 
    d1.pub_mem = 1;     //正确
//    d1.pro_mem = 2;     //错误，在派生类中pro_mem的访问属性是protected
//    d1.pri_mem = 3;     //错误，在派生类中pri_mem的访问属性是private

//    Pro_Derv d1;
//    d1.pub_mem = 1;     //错误，在派生类中pub_mem的访问属性是protected
//    d1.pro_mem = 2;     //错误，在派生类中pro_mem的访问属性是protected
//    d1.pri_mem = 3;     //错误，在派生类中pri_mem的访问属性是private
//
//    Pri_Derv d2;
//    d2.pub_mem = 1;     //错误，在派生类中pub_mem的访问属性是private
//    d2.pro_mem = 2;     //错误，在派生类中pro_mem的访问属性是private
//    d2.pri_mem = 3;     //错误，在派生类中pri_mem的访问属性是private

    return 0;
}


