#include <boost/smart_ptr.hpp>
#include <iostream>

using namespace std;
using namespace boost;
class node_son
{
public:
	node_son(int v):x(v){};
        ~node_son()
        {       cout << "son deleted" << x << endl;}
        int x;
};
class node
{
public:
	node(int v):x(v), son(v * 10)
	{}
	~node()
	{	cout << "deleted" << x << endl;}
	int x;

	typedef shared_ptr<node> ptr_type;
	ptr_type next;
	node_son son;
};
/* 智能指针循环引用为什么导致　智能指针失效呢，搞不懂
 
回答：因为内部p1->next需要引用计数为0的时候才会删除 p1 与 p1-next
退出的时候析构顺序如下:
首先p1计数器 -1，因为p2-next = p1的原因 p1.use_count() = 1 p1 不会析构，因此p1->next依然有效
然后p2计数器 -1 因为p1->next = p2依然有效 p2也不会自动析构
这样就存在内存泄露的情况了。
我之前的错误分析想法也记录一下:
 首先 p1 计数器 -1， 然后 p1->next释放 p2 计数器 -1，
 然后 p2 计数器 -1， 然后 p2->next释放 p1 计数器 -1，

*/
int main()
{

	shared_ptr<node> p1(new node(1)); //make_shared<node>();
	shared_ptr<node> p2(new node(2)); //make_shared<node>();
	cout << "p1 use:" << p1.use_count() << endl;
	cout << "p2 use:" << p2.use_count() << endl;

	p1->next = p2;
	p2->next = p1;
        cout << "p1 use:" << p1.use_count() << endl;
        cout << "p2 use:" << p2.use_count() << endl;
	//p1->next.reset();
	//p1.reset();
        cout << "p1 use:" << p1.use_count() << endl;
        cout << "p2 use:" << p2.use_count() << endl;
	//p2->next.reset();
	//p2.reset();
	cout << "p1 use:" << p1.use_count() << endl;
        cout << "p2 use:" << p2.use_count() << endl;
	node s(3);

}

