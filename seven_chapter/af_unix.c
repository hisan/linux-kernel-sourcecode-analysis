#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define MYSOCK_ADDR "./un_socket"
#define LISTEN_QUEUE 100

int delete_socket(const char *deststring)
{
	struct stat fsstat;
	if (stat(deststring,&fsstat) == -1)
	{
		printf("there is no socket file!\r\n");
		return 0;
	}
	else 
	{
		if (-1 == unlink(deststring))
		{
			printf("rm %s failed!\r\n",deststring);
			return 1;
		}
	}
	
	return 0;
}

int main()
{
	int ret;
	int sockfd = 0;
	struct sockaddr_un servaddr,client_addr;
	socklen_t socklen;
	int socksize = sizeof(struct sockaddr_un);	
	
	sockfd = socket(AF_UNIX,SOCK_STREAM,0);
	if (!sockfd)
	{
		printf("get socket failed!\r\n");
		return 0;
	}
	
	memset(&servaddr,0,socksize);
	servaddr.sun_family = AF_UNIX;
	strncpy(servaddr.sun_path,MYSOCK_ADDR,sizeof(servaddr.sun_path) -1);
	
	if (delete_socket(MYSOCK_ADDR))
	{
		printf("delete socket file failed!\r\n");
		return 0;
	}
	

	if (bind(sockfd,(struct sockaddr *)&servaddr,socksize) == -1)
	{
		printf("bind socket failed!\r\n");
		return 0;
	}
	
	if (listen(sockfd,LISTEN_QUEUE) == -1)
	{
		printf("listen failed!\r\n");
		return 0;
	}
	
	while (1)
	{
		memset(&client_addr,0,socksize);
		ret = accept(sockfd,(struct sockaddr *)&client_addr,(socklen_t *)&socklen);
		if (ret == -1)
		{
			printf("connect receive!\r\n");
		}
		else 
		{
			printf("connect receive:[%s]!\r\n",client_addr.sun_path);
		}
		/* handle the receive things */
	}
	
	return 0;
}