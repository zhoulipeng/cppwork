#include<map>
#include<string>
#include<iostream>
#include<sstream>
using namespace std;
// 先erase 后 it++ 在c++ 里是不安全行为
// 测试情况是: 在ubuntu 12.04 g++ 4.6.3, 没有问题
// 但是在 centos stream 8 g++ 8.5, 有崩溃问题
//
int main()
{
    multimap<int,string> m;
    int cnt = 0;

    stringstream ss;
    for(int i=0; i < 100; i++) {
        ss << i;
        m.insert(pair<int,string>(i-100, ss.str()));
        ss.str(""); // clear data
    }

    for(int i=0; i < 100; i++) {
        ss << i;
        m.insert(pair<int,string>(i, ss.str()));
        ss.str(""); // clear data
    }
    
    multimap<int,string>::iterator it;
    for(it=m.begin();it!=m.end();)
    {
        //cout<<"key: "<<it->first <<" value: "<<it->second<<endl;
        cnt++;
        m.erase(it++);
    }
    cout << "cnt:" << cnt << endl;
    return 0;
}
