#include <cstdio>
#include <boost/pool/singleton_pool.hpp>
using namespace boost;

struct pool_tag{int memb;};
typedef singleton_pool<pool_tag, sizeof(int)> spl;

int main()
{
	printf("%d\n", sizeof(struct pool_tag));
	int *p = (int *) spl::malloc();
	assert(spl::is_from(p));
	spl::release_memory();
}
