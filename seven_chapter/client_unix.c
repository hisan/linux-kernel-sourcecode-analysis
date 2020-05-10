/*
	2020-05-10 º¼ÖÝ  client
*/

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>

#define MYSOCK_ADDR "./un_socket"

int main()
{
	int sockfd;
	int addrsize;
	struct sockaddr_un servaddr,clientaddr;
	int ret;
	
	sockfd = socket(AF_UNIX,SOCK_STREAM,0);
	if (sockfd == -1)
	{
		printf("create socket failed!\r\n");
		return 0;
	}
	
	addrsize = sizeof(struct sockaddr_un);
	
	memset(&clientaddr,0,addrsize);
	clientaddr.sun_family = AF_UNIX;
	strncpy(clientaddr.sun_path,MYSOCK_ADDR,sizeof(clientaddr.sun_path) -1);
	
	if (bind(sockfd,(struct sockaddr *)&clientaddr,addrsize) == -1)
	{
		printf("bind socket failed!\r\n");
		return 0;
	}
	
	
	memset(&servaddr,0,addrsize);
	servaddr.sun_family = AF_UNIX;
	strncpy(servaddr.sun_path,MYSOCK_ADDR,sizeof(servaddr.sun_path) -1);
	
	ret = connect(sockfd,(struct sockaddr *)&servaddr,(socklen_t)addrsize);
	if (ret != 0)
	{
		printf("connect server:[%s] ret:[%d] failed!\r\n",MYSOCK_ADDR,ret);
		return 0;
	}
	
	return 0;
}