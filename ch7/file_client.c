#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main()
{
    int sock;
    FILE *fp;
    char buf[BUF_SIZE];
    int read_cnt;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    fp = fopen("receive.dat","wb");

    char * serv_ip = "127.0.0.1";
    char * serv_port = "9190";
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
    serv_addr.sin_port = htons(atoi(serv_port));

    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1) 
        error_handling("socket() error!");
    
    if( connect(sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1 )
        error_handling("connect() error!");
    else printf("connect to server...\n");

    while ((read_cnt=read(sock,(void*)buf,BUF_SIZE))!=0)
    {
        fwrite((void*)buf,1,read_cnt,fp);
    }

    char thank_message[] = "thank you";
    printf("Received data!\n");
    write(sock,thank_message,BUF_SIZE);
    fclose(fp);
    close(sock);
    

    return 0;
}

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}