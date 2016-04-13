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
//智能指针循环引用为什么导致　智能指针失效呢，搞不懂
//
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

