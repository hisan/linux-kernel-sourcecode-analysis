2020-05-06 hangzhou zhejaing

ͨ��˫�����෢����Ϣ
1.������
	read()/write()
	
2.��Ҫ���������ӵ���Ϣ���ݵĽӿ�
	recv()/send()
	recvfrom()/sendto()
	recvmsg()/sendmsg()
	
	

unix_stream_ops



//unix���ַ�ṹ
#define UNIX_PATH_MAX 108
struct unix_address{
	atomic_t refcnt;
	int len;
	unsigned hash;
	struct sockaddr_un name[0];//�䳤�����Ӧ��:Ϊ�˲��˷��ڴ�ռ�
}


struct sockaddr_un{
	sa_family_t sun_family;
	char sun_path[UNIX_PATH_MAX];
}