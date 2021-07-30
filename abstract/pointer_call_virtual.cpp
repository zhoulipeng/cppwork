// LISTING 12.10 - Deriving Abstract Classes from Other Abstract Classes
/*
http://www.cnblogs.com/zhengyuhong/archive/2011/12/25/2300837.html
在这里,我们看到哺乳动物继承了动物,然后将繁殖Reproduce()变成非纯虚函数, 
其他依然保持纯虚函数,因为哺乳动物的繁殖方式是一样的,所以应该将繁殖方式
变成非纯虚函数,而其他函数可能不一样.由于Mammal含有纯虚函数,所以这个依
然是抽象类,不能实例化
*/
#include <iostream>
using namespace std;

// 
class Animal // common base to both Mammal and Fish
{
public:
    Animal();
    virtual ~Animal() { cout << age << "Animal destructor...\n"; }
    virtual void Sleep() const { cout << age << "Animal Sleep\n"; }
    void Speak() { cout << age << "Animal Speak\n"; }
    // 纯虚函数的意义在于不可以实例化该类，且不用写"{}"这样无意义的东西，并且要求其某个派生类必然要实现该接口 
    int age = 1;
};

Animal::Animal()
{
    //cout << "Animal constructor...\n";
}

class Mammal : public Animal
{
public:
    Mammal()
    {//cout << "Mammal constructor...\n";
        age = 11;
    }
    void Sleep() const { cout << age << "Mammal Sleep\n"; }
    void Speak() { cout << age << "Mammal Speak\n"; }
};

int main()
{
    Animal *p1 = new Mammal();
    Mammal *pm = (Mammal *)p1;
    p1->Sleep();// 1
    p1->Speak();// 2

    pm->Sleep();// 3
    pm->Speak();// 4
    Animal *p2 = new Animal();
    pm = (Mammal *)p2;
    p2->Sleep();// 5
    p2->Speak();// 6
    pm->Sleep();// 7
    /**************************/
    pm->Speak();// 8 这里为什么打印的是子类的方法

    /***********总结***************/
    /*
     * age 只和new实际创建的类有关
     * 1 3 5 7 打印我们可以看到, virtual 方法,永远和创建的类有关
     * 2 4 6 8 打印我们可以看到, 重写继承的方法,永远和当前的指针类型有关, 
     * 特别注意第8个打印
     */
    delete p1;
    delete p2;
    return 0;
}
