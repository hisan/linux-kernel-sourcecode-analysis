#include <stdio.h>

#define PAGE_SHIFT 	12

int main()
{
	printf("%d\r\n",((16 * 1024 * 1024) >> PAGE_SHIFT));
	return 0;
}

/* 4096 */