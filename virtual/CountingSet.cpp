//http://www.cnblogs.com/gnuhpc/archive/2012/01/17/2324836.html
#include <iostream>

using std::endl;
using std::cout;

 
class Set {
    std::set<int> s_;
  public:
    void add(int i) {
      s_.insert(i);
      add_impl(i); // Note virtual call.
    }
    void addAll(int * begin, int * end) {
      s_.insert(begin, end);   //  --------- (1)
      addAll_impl(begin, end); // Note virtual call.
    }
  private:
    virtual void add_impl (int i) = 0;
    virtual void addAll_impl (int * begin, int * end) = 0;
};
class CountingSet : public Set {
  private:
    int count_;
    virtual void add_impl (int i) {
      count_++;
    }
    virtual void addAll_impl (int * begin, int * end) {
      count_ += std::distance(begin,end);
    }
};

int main(int argc, char *argv[])
{
	Derived b;
	b.Foo();
} 
