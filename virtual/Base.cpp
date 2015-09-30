//http://www.cnblogs.com/gnuhpc/archive/2012/01/17/2324836.html
#include <iostream>

using std::endl;
using std::cout;

class Base{
 public:
     void Foo(){
         DoFoo1();
         DoFoo2();
     }//use DoFooX()
 private:
     virtual void DoFoo1(){
         cout << "Base's DoFoo1" <<endl;
     }
     virtual void DoFoo2(){
         cout << "Base's DoFoo2" <<endl;
     }
 };
 
class Derived: public Base{
 private:
     virtual void DoFoo1(){
         cout << "Derived's DoFoo1" << endl;
     };
};

int main(int argc, char *argv[])
{
	Derived b;
	b.Foo();
} 
