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
        using Base::fun;
        void fun(double data, int o)
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
