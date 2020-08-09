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

	struct sockaddr_in serv_addr, from_addr;

	char message1[] = "hello host1";
    char message2[] = "I'm host2";
    char message3[] = "Nice to meet you";

	sock = socket(PF_INET, SOCK_DGRAM,0);
	if(sock == -1) 
		error_handling("socket() error");

	char* serv_ip = "127.0.0.1";
	char* serv_port = "9190";

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

    sendto(sock,message1,BUF_SIZE,0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    printf("send message: %s\n",message1);
    sendto(sock,message2,BUF_SIZE,0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    printf("send message: %s\n",message2);
    sendto(sock,message3,BUF_SIZE,0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    printf("send message: %s\n",message3);

	close(sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}