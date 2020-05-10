#include <stdio.h>

#define AL(x) ((x) * sizeof(unsigned long))
static unsigned char nargs[18]={AL(0),AL(3),AL(3),AL(3),AL(2),AL(3),
				AL(3),AL(3),AL(4),AL(4),AL(4),AL(6),
				AL(6),AL(2),AL(5),AL(5),AL(3),AL(3)};
#undef AL


int main()
{
	int times = sizeof(nargs)/sizeof(nargs[0]);
	
	for (int i = 0; i < times; i++)
	{
		printf("nargs[%d]:%d\r\n",i,nargs[i]);
	}
	return 0;
}

/* 
nargs[0]:0
nargs[1]:12
nargs[2]:12
nargs[3]:12
nargs[4]:8
nargs[5]:12
nargs[6]:12
nargs[7]:12
nargs[8]:16
nargs[9]:16
nargs[10]:16
nargs[11]:24
nargs[12]:24
nargs[13]:8
nargs[14]:20
nargs[15]:20
nargs[16]:12
nargs[17]:12
 */