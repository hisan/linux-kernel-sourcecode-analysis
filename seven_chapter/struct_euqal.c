#include <stdio.h>

struct test{
	int a;
	int (*test)(int,int);
};

int test_fun(int a,int b)
{
	a ^= b;
	b ^= a;
	a ^= b;
	return 0;
}

int main()
{
	struct test st_test = {
		a:10,
		test:test_fun,
	};
	printf("%d:%d!\r\n",st_test.a,st_test.test(10,20));
}