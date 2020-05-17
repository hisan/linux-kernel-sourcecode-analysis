/*
	2020-05-10 º¼ÖÝ  client
*/

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MYSOCK_ADDR "./un_socket"
#define BUFFER_MAX 1024 

int main()
{
	int sockfd;
	int addrsize;
	struct sockaddr_un servaddr,clientaddr;
	int ret;
	char buffer[BUFFER_MAX] = {0};
	int wrlen = 0,left = 0;
	
	sockfd = socket(AF_UNIX,SOCK_STREAM,0);
	if (sockfd == -1)
	{
		printf("create socket failed!\r\n");
		return 0;
	}
	
	addrsize = sizeof(struct sockaddr_un);
	
	memset(&servaddr,0,addrsize);
	servaddr.sun_family = AF_UNIX;
	strncpy(servaddr.sun_path,MYSOCK_ADDR,sizeof(servaddr.sun_path) -1);
	
	ret = connect(sockfd,(struct sockaddr *)&servaddr,(socklen_t)addrsize);
	if (ret != 0)
	{
		printf("connect server:[%s] ret:[%d] failed!\r\n",MYSOCK_ADDR,ret);
		return 0;
	}
	
	strncpy(buffer,"this is client!\r\n",strlen("this is client!\r\n"));
	
	wrlen = write(sockfd,buffer+wrlen,strlen(buffer+wrlen));
	left  = strlen(buffer) - wrlen;
	
	while (left)
	{
		wrlen += write(sockfd,buffer+wrlen,strlen(buffer+wrlen));
		left  = strlen(buffer) - wrlen;
	}
	
	return 0;
}