#include <boost/smart_ptr.hpp>
#include <string>
#include <iostream>
#include <cstdio>
using namespace boost;
using namespace std;
struct posix_file
{
	posix_file(const char * file_name)
	{	cout << "Open file:" << file_name << endl;	}
	~posix_file()
	{	cout << "close file" << endl; }
};

int main()
{
	scoped_ptr<int> p(new int);
	if(p)					//在bool语境中测试指针是否有效
	{
		*p = 100;
		cout << *p << endl;
	}

	p.reset();
	assert(p == NULL);
	if(!p)
	{	cout << "scoped_ptr == null" << endl; }
	//文件类的scoped_ptr;
	//将在离开作用域时自动析构，从而关闭文件释放资源
	scoped_ptr<posix_file> fp(new posix_file("/tmp/a.txt"));
}//在这里发生scoped_ptr的析构,p 和 fp管理的指针自动被删除
