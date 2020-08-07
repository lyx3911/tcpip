#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void ErrorHandling(char *message);

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    SOCKET hSocket;
    SOCKADDR_IN serv_addr;

    char message[30];

    if(argc!=3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0)
        ErrorHandling("WSADStartup() error");

    hSocket = socket(PF_INET,SOCK_STREAM, 0);
    if(hSocket == INVALID_SOCKET)
        ErrorHandling("SOCKET() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
    
    if(connect(hSocket,(struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		ErrorHandling("connect() error!");

	int str_len = recv(hSocket, message, sizeof(message)-1, 0);
	if(str_len==-1)
		ErrorHandling("read() error");

	printf("Message from sever: %s\n", message);
	closesocket(hSocket);
    WSACleanup(); 

    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}