#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OPSZ 4
#define RLT_SIZE 4

void error_handling(char *message);
int calculate(int opnum, int opnds[]);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	// if(argc!=2)
	// {
	// 	printf("Usage: %s <port> \n", argv[0]);
	// 	exit(1);
	// }

	serv_sock = socket(PF_INET, SOCK_STREAM,0);
	if(serv_sock == -1) 
		error_handling("socket() error");
	
	char* serv_port = "9190";
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(serv_port));

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1 )
		error_handling("bind() error");



	if(listen(serv_sock, 5)== -1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);


	for(int i=0;i<5;i++) //处理5个客户端连接
	{
		printf("wait for connect req...\n");
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
		if(clnt_sock==-1)
			error_handling("accept() error");

		else printf("accept client %d\n", i+1);

		int opnum;
		int opnds[9];

		read(clnt_sock,&opnum,1);//先读取有几个数

		for(int i=0;i<opnum;i++)
		{
			read(clnt_sock,&opnds[i],4);
			printf("%d ",opnds[i]);
		}
		printf("\n");

		int result = calculate(opnum,opnds);
		write(clnt_sock,(char*)&result,sizeof(result));

		close(clnt_sock);
	}
	

	close(serv_sock);

	return 0;
}

int calculate(int opnum, int opnds[])
{
	int sum = 0;

	for(int i=0;i<opnum;i++)
	{
		sum += opnds[i];
	}

	return sum;
}

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}