#include <iostream>
#include <map>
#include <utility>
using namespace std;

class Sample
{
    static int _noOfObjects;

public:
    int _objectNo;
    Sample()
        : _objectNo(_noOfObjects++) {
        std::cout << "Inside default constructor of object " << _objectNo << std::endl;
    }

    Sample(const Sample& sample)
        : _objectNo(_noOfObjects++) {
        std::cout << "Inside copy constructor of object " << _objectNo << ", from: " << sample._objectNo << std::endl;
    }

    Sample& operator=(const Sample& s) {
        std::cout << "operator=" << _objectNo << ", from: " << s._objectNo << std::endl;
        return *this;
    }

    ~Sample() {
        std::cout << "Destroying object " << _objectNo << std::endl;
    }
};
int Sample::_noOfObjects = 0;


int main(int argc, char* argv[]) {

    Sample sample;
    std::map<int, Sample> map;

    cout << "after map:" << endl;
    pair<int, Sample> tmp(1, sample);

    cout << "after tmp:" << endl;
    //map.insert(std::make_pair(1, sample));
    //map.insert(pair<int, Sample>(1, sample));
    map.insert(tmp);
    //map.emplace(pair<int, Sample>(1, sample)); // c++11
    //map[1] = sample;
    std::cout << "return 0" << std::endl;
    return 0;
}


/*
-----  c++98 c++03  insert make_pair
Inside default constructor of object 0
Inside copy constructor of object 1
Inside copy constructor of object 2
Inside copy constructor of object 3
Inside copy constructor of object 4
Destroying object 3
Destroying object 2
Destroying object 1
return 0
Destroying object 4
Destroying object 0

----- c++98 c++03 insert pair<>
Inside default constructor of object 0
Inside copy constructor of object 1
Inside copy constructor of object 2
Inside copy constructor of object 3
Destroying object 2
Destroying object 1
return 0
Destroying object 3
Destroying object 0
0 代表 sample
1 代表栈内存上的 pair<> 临时对象
2 代表insert 函数形参栈内存上的pair 对象
3 代表map 内部二叉树堆内存的pair 对象


----- c++98 c++03 insert by index
Inside default constructor of object 0
Inside default constructor of object 1
Inside copy constructor of object 2
Inside copy constructor of object 3
Destroying object 2
Destroying object 1
operator=
return 0
Destroying object 3
Destroying object 0


----- c++11 c++14  emplace and insert<> and insert make_pair 一样的结果
Inside default constructor of object 0
Inside copy constructor of object 1
Inside copy constructor of object 2
Destroying object 1
return 0
Destroying object 2
Destroying object 0
----- c++11 c++14 insert by index
Inside default constructor of object 0
Inside default constructor of object 1
operator=
return 0
Destroying object 1
Destroying object 0
 
*/
