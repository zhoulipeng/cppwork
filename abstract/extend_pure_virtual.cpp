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

class Animal // common base to both Mammal and Fish
{
public:
    Animal(int);
    virtual ~Animal() { cout << "Animal destructor...\n"; }
    virtual int GetAge() const { return itsAge; }
    virtual void SetAge(int age) { itsAge = age; }
    virtual void Sleep() const = 0;
    virtual void Eat() const = 0;
    virtual void Reproduce() const = 0;
    virtual void Move() const = 0;
    virtual void Speak() const = 0;
private:
    int itsAge;
};

Animal::Animal(int age):
itsAge(age)
{
    cout << "Animal constructor...\n";
}

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
      virtual void Move() const { cout << "Horse running...\n";}

protected:
    COLOR itsColor;
};

class Dog : public Mammal
{
public:
    Dog(int age, COLOR color ):
      Mammal(age), itsColor(color)
      { cout << "Dog constructor...\n"; }
      virtual ~Dog() { cout << "Dog destructor...\n"; }
      virtual void Speak()const { cout << "Whoof!... \n"; }
      virtual void Sleep() const { cout << "Dog snoring...\n"; }
      virtual void Eat() const { cout << "Dog eating...\n"; }
      virtual void Move() const { cout << "Dog running...\n"; }
      virtual void Reproduce() const
      { cout << "Dogs reproducing...\n"; }

protected:
    COLOR itsColor;
};

int main()
{
    Animal *pAnimal=0;
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
            pAnimal->Speak();
            pAnimal->Eat();
            pAnimal->Reproduce();
            pAnimal->Move();
            pAnimal->Sleep();
            delete pAnimal;
            cout << endl;
        }
    }
    return 0;
}
