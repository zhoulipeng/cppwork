#include <boost/smart_ptr.hpp>
#include <string>
#include <iostream>
using namespace boost;
using namespace std;
//拷贝构造函数与赋值构造函数声明为私有，因此scoped_ptr参数智能传递引用
int call_func_withscoped_ptr(scoped_ptr<string> &p)
{
	cout << *p << endl;
        cout << p->size() << endl;
}
int main()
{
	scoped_ptr<string> sp(new string("text"));
	scoped_ptr<string> sp2(new string("text2"));
	cout << *sp << endl;
	cout << sp->size() << endl;
	call_func_withscoped_ptr(sp);
	sp.reset(new string("reset string"));
	cout << *sp << endl;
        cout << sp->size() << endl;

}
