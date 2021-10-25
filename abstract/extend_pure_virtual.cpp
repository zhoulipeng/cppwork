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

enum COLOR { Red, Green, Blue, Yellow, White, Black, Brown } ;
// 
class Animal // common base to both Mammal and Fish
{
public:
    Animal(int);
    virtual ~Animal() { cout << "Animal destructor...\n"; }
    virtual int GetAge() const { return itsAge; }
    virtual void SetAge(int age) { itsAge = age; }
    virtual void Sleep() const = 0;
    // 纯虚函数的意义在于不可以实例化该类，且不用写"{}"这样无意义的东西，并且要求其某个派生类必然要实现该接口 
    virtual void Eat() const = 0;
    virtual void EatHow(int weight) const ; // 如果EatHow是纯虚函数，则不能在子类Dog中用using 的方式引入，因为using不是真正的继承实现，而虚函数则要求子类必须实现 
    virtual void Reproduce() const = 0;
    virtual void Move() const = 0;
    virtual void Speak() const = 0;
private:
    int itsAge;
};

// 纯虚函数可以有代码实现，但必须定义在类申明的外面,且不改变虚函数性质
void Animal::Eat() const {
    cout << "Animal eat food." << endl;
}

void Animal::EatHow(int weitht) const {
    cout << "Animal need eat "  << weitht << "kg food"<< endl;
}

Animal::Animal(int age):
itsAge(age)
{
    cout << "Animal constructor...\n";
}
// [c++]为什么可以在基类中实现纯虚函数
// https://blog.csdn.net/fnzsjt/article/details/38350681
// 在<<Effective C++>>(没有拜读过)讲明，派生类可以显式地调用基类中的纯虚函数，这样就可以将将不同子类中公共的事务放在父类中完成
// 
void Animal::Sleep() const
{
    cout << "virtual Sleep\n";
//  比如父类有void method1(), 子类也有void method1(), 那么当对象定义为子类时,
//  执行method1时只会执行子类的method1, 父类的method1不执行, 那么也要同时执行父类的method1怎么处理, 
//  可以在子类代码里面显式调用"父类::method1()" . 
//  原文：https://blog.csdn.net/rocklee/article/details/75148971 
}
// 继承抽象类（带有纯虚函数的类），如果不实现所有纯虚函数，也不能实例化，只能定义指针
class Mammal : public Animal
{
public:
    Mammal(int age):Animal(age)
    { cout << "Mammal constructor...\n";}
    virtual ~Mammal() { cout << "Mammal destructor...\n";}
    virtual void Reproduce() const
    { cout << "Mammal reproduction depicted...\n"; }
};

class Fish : public Animal
{
public:
    Fish(int age):Animal(age)
    { cout << "Fish constructor...\n";}
    virtual ~Fish() {cout << "Fish destructor...\n"; }
    virtual void Sleep() const { cout << "fish snoring...\n"; }
    virtual void Eat() const { cout << "fish feeding...\n"; }
    virtual void EatHow(int weight) const { cout << "fish feeding..." << weight/5 << endl; }
    virtual void Reproduce() const
    { cout << "fish laying eggs...\n"; }
    virtual void Move() const
    { cout << "fish swimming...\n"; }
    virtual void Speak() const { }
};

class Horse : public Mammal
{
public:
    Horse(int age, COLOR color ):
    Mammal(age), itsColor(color)
    { cout << "Horse constructor...\n"; }
    virtual ~Horse() { cout << "Horse destructor...\n"; }
    virtual void Speak()const { cout << "Whinny!... \n"; }
    virtual COLOR GetItsColor() const { return itsColor; }
    virtual void Sleep() const
    { cout << "Horse snoring...\n"; }
    virtual void Eat() const { cout << "Horse feeding...\n"; }
    virtual void EatHow(int weight) const { cout << "horse  nead eating ..." << weight << endl; }
    virtual void Move() const { cout << "Horse running...\n";}

protected:
    COLOR itsColor;
};

class Dog : public Mammal
{
public:
/* see ./DataHiding.cpp
关于using 的注释
其实，并没有‘子类“重载”父类的方法’这种说法。
《C++Primer Plus》 中说道：
如果基类声明被重载了，则应该在派生类中重新定义所有的基类版本。
如果在派生类中只重新定义一个版本，其他版本将会被隐藏，派生类对象将无法使用它们
*/
   // 如果子类重载父类同名的方法,参数列表个数不一样，编译器会搜索子类符合条件的方法(导致隐藏)，不会搜索父类方法(见DataHiding2.cpp)
    using Animal::EatHow;
    Dog(int age, COLOR color ):
    Mammal(age), itsColor(color)
    { cout << "Dog constructor...\n"; }
    virtual ~Dog() { cout << "Dog destructor...\n"; }
    virtual void Speak()const { cout << "Whoof!... \n"; }
    // 如果在派生类中显式的调用基类中的纯虚函数，则基类必须实现纯虚函数接口的函数体
    virtual void Sleep() const { cout << "Dog snoring...\n"; Animal::Sleep();}
    virtual void Eat() const { 
        Animal::Eat();
        cout << "Dog eating...\n"; }
    virtual void EatHow(double weight) const { 
        cout << "Dog  nead eating(type:double) ..." << weight/5 << endl; }

    virtual void Move() const { cout << "Dog running...\n"; }
    virtual void Reproduce() const
    { cout << "Dogs reproducing...\n"; }

protected:
    COLOR itsColor;
};

int main()
{
    Animal *pAnimal=0;
    //Mammal obj; //不能实例化
    Mammal *obj = nullptr;
    int choice;
    bool fQuit = false;

    while (fQuit == false)
    {
        cout << "(1)Dog (2)Horse (3)Fish (0)Quit: ";
        cin >> choice;

        switch (choice)
        {
        case 1: pAnimal = new Dog(5,Brown);
            break;
        case 2: pAnimal = new Horse(4,Black);
            break;
        case 3: pAnimal = new Fish (5);
            break;
        default: fQuit = true;
            break;
        }
        if (fQuit == false)
        {
            pAnimal->SetAge(choice);
            cout << pAnimal->GetAge() << endl;
            pAnimal->Speak();
            pAnimal->Eat();
            pAnimal->EatHow(10);
            if (choice == 1 ) {
                ((Dog*)pAnimal)->EatHow(10);// 如果dog 类中不是用using 使父类 int 版本可见，那么 
            }
            pAnimal->Reproduce();
            pAnimal->Move();
            pAnimal->Sleep();
            delete pAnimal;
            cout << endl;
        }
    }
    return 0;
}
