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
	int serv_sock;

	struct sockaddr_in serv_addr, clnt_addr;

	char message[BUF_SIZE];

	serv_sock = socket(PF_INET, SOCK_DGRAM,0);
	if(serv_sock == -1) 
		error_handling("socket() error");

	char* serv_ip = "127.0.0.1";
	char* serv_port = "9190";

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(serv_port));

    if(bind(serv_sock,(sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("bind() error");
    
    //与TCP相比，不需要accept和listen
    while(true)
    {
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        int str_len = recvfrom(serv_sock,message,BUF_SIZE,0,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
        fputs(message,stdout);
        sendto(serv_sock,message,BUF_SIZE,0,(struct sockaddr*)&clnt_addr,clnt_addr_size);
    }	

	
	close(serv_sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}