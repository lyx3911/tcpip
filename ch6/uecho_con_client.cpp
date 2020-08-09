#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;

	struct sockaddr_in serv_addr;

	char message[BUF_SIZE];

	sock = socket(PF_INET, SOCK_DGRAM,0);
	if(sock == -1) 
		error_handling("socket() error");

	char* serv_ip = "127.0.0.1";
	char* serv_port = "9190";

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));
    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)); //创建已连接的UDP套接字


    while(true)
    {
        printf("insert message(q to quit): ");
        fgets(message,sizeof(message),stdin);
        if(!strcmp(message,"q\n")) {
            printf("quit\n");
            break;
        }

        write(sock,message,BUF_SIZE);
        char recv_message[BUF_SIZE];
        read(sock,recv_message,BUF_SIZE);
        printf("receive message: %s\n",recv_message);
    }	

	
	close(sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}