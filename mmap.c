#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int flag = 0;
	int fd = 0;
	flag = O_CREAT;
	char *pmamp = NULL;
	
	struct stat filestat;
	memset(&filestat,0,sizeof(struct stat));
	
	fd = open("./test.txt",flag);
	
	if (fd < 0)
	{
		printf("open file failed!\n");
	}
	
	fstat(fd,&filestat);//st_size
	
	pmamp  = mmap(NULL, filestat.st_size+10, PROT_WRITE,  MAP_PRIVATE,fd, 0);
	
	printf("%s\n",pmamp);
	
}

/*
 addr = mmap(NULL, length + offset - pa_offset, PROT_READ,
                       MAP_PRIVATE, fd, pa_offset);
*/