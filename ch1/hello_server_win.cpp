#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void ErrorHandling(char *message);

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    SOCKADDR_IN servAddr, clntAddr;
    int szClntAddr;
    char message[] = "hello world";

    if(argc!=2)
    {
        printf("Usage: %s <prot>\n",argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0) //初始化
        ErrorHandling("WSAStartup() error");
    
    hServSock = socket(PF_INET, SOCK_STREAM, 0); //创建套接字，成功返回套接字句柄，失败返回 INVALID_SOCKET
    if(hServSock==INVALID_SOCKET)
        ErrorHandling("SOCKET() error");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if(bind(hServSock, (SOCKADDR* )&servAddr, sizeof(servAddr))==SOCKET_ERROR)
        ErrorHandling("BIND() error");
    
    if(listen(hServSock, 5)==SOCKET_ERROR)
        ErrorHandling("LISTEN() error");
    
    szClntAddr = sizeof(clntAddr);
    hClntSock = accept(hServSock,(SOCKADDR*)&clntAddr,&szClntAddr);
    if(hClntSock==INVALID_SOCKET) 
        ErrorHandling("ACCEPT() error");
    
    send(hClntSock, message, sizeof(message), 0);
    closesocket(hClntSock);
    closesocket(hServSock);
    WSACleanup();
        
    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}