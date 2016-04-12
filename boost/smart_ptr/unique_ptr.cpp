#include <memory>
#include <string>
#include <iostream>
#include <cstdio>
#include <cassert>
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
	unique_ptr<int> up(new int);
	assert(up);
	*up  = 10;
	cout << *up << endl;
	up.reset();
	assert(!up);
}
