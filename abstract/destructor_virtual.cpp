#include <iostream>

using namespace std;

class Base
{
public:
    virtual ~Base() { cout << "/* releases Base's resources */" << endl; }
};

class Derived : public Base
{
public:
    ~Derived() { cout << "/* releases Derived's resources */" << endl; }
};

class Derived2 : public Derived 
{
    ~Derived2() { cout << "/* releases Derived2's resources */" << endl; }
};
int main()
{
    Base* b = new Derived2;
    delete b; // Makes a virtual function call to Base::~Base()
              // since it is virtual, it calls Derived::~Derived() which can
              // release resources of the derived class, and then calls
              // Base::~Base() following the usual order of destruction

    Derived* d = new Derived2;
    delete d;// virtual 具有传递性， 孙子类也是virtual
}

// g++ -std=c++98 extends2.cpp -o extends2.exe
