#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int clnt_sock;

	struct sockaddr_in serv_addr;

	// char message[BUF_SIZE];

	if(argc!=3)
	{
		printf("Usage: %s <port> \n", argv[0]);
		exit(1);
	}

	clnt_sock = socket(PF_INET, SOCK_STREAM,0);
	if(clnt_sock == -1) 
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));


	// clnt_addr_size = sizeof(clnt_addr);


	if(connect(clnt_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		error_handling("connect() error");

	else printf("connected...\n");

	while(true)
	{
		char message[BUF_SIZE];
		fputs("Input message(Q to quit): ",stdout);
		fgets(message,BUF_SIZE,stdin);

		if(!strcmp(message,"q\n")|| !strcmp(message,"Q\n"))
		{
			printf("quit\n");
			break;
		}

		write(clnt_sock,message,strlen(message));
		int str_len = read(clnt_sock,message,BUF_SIZE-1);
		message[str_len]=0;
		printf("message from server %s\n", message);
	}
	
	close(clnt_sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}