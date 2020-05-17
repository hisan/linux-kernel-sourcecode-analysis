2020-05-06 hangzhou zhejaing

通信双方互相发送信息
1.有连接
	read()/write()
	
2.主要用于无连接的信息传递的接口
	recv()/send()
	recvfrom()/sendto()
	recvmsg()/sendmsg()
	
	

unix_stream_ops



//unix域地址结构
#define UNIX_PATH_MAX 108
struct unix_address{
	atomic_t refcnt;
	int len;
	unsigned hash;
	struct sockaddr_un name[0];//变长数组的应用:为了不浪费内存空间
}


struct sockaddr_un{
	sa_family_t sun_family;
	char sun_path[UNIX_PATH_MAX];
}