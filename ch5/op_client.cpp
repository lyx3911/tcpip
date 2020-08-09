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

int main(int argc, char *argv[])
{
	int clnt_sock;

	struct sockaddr_in serv_addr;

	char message[BUF_SIZE];

	// if(argc!=3)
	// {
	// 	printf("Usage: %s <port> \n", argv[0]);
	// 	exit(1);
	// }

	clnt_sock = socket(PF_INET, SOCK_STREAM,0);
	if(clnt_sock == -1) 
		error_handling("socket() error");

	char* serv_ip = "127.0.0.1";
	char* serv_port = "9190";

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));


	// clnt_addr_size = sizeof(clnt_addr);


	if(connect(clnt_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		error_handling("connect() error");

	else printf("connected...\n");

	int result,opnd_cnt;

	//读取有多少个操作数
	printf("Operand count: ");
	scanf("%d",&opnd_cnt);
	message[0] = (char)opnd_cnt;

	//读取每个数
	for(int i=0;i<opnd_cnt;i++)
	{
		printf("Operand %d:",i+1);
		scanf("%d",(int*)&message[i*OPSZ+1]);
	}

	fgetc(stdin);
	write(clnt_sock,message,BUF_SIZE-1);
	read(clnt_sock,&result,sizeof(result));
	printf("sum: %d\n",result);

	
	close(clnt_sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}