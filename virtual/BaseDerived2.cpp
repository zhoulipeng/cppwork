//http://www.cnblogs.com/gnuhpc/archive/2012/01/17/2324836.html
#include <iostream>

using std::endl;
using std::cout;

class Base{
public:
    virtual void Foo(){
        cout << "Locking" << endl;
        cout << "Base's Foo" << endl;
        cout << "Unlocking" << endl;
    }
};

class Derived: public Base{
public:
    virtual void Foo(){
        cout << "Locking" << endl;
        cout << "Derived's Foo" << endl;
        cout << "Unlocking" << endl;
    };
};

int main(int argc, char *argv[])
{
	Derived b;
	b.Foo();
} 
