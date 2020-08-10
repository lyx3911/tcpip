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
    int serv_sock, clnt_sock;
    FILE *fp;
    char buf[BUF_SIZE];
    int read_cnt;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    fp = fopen("file_server.c","rb");

    char * serv_ip = "127.0.0.1";
    char * serv_port = "9190";
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
    serv_addr.sin_port = htons(atoi(serv_port));

    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1) 
        error_handling("socket() error!");

    if( bind(serv_sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1 )
        error_handling("bind() error!");
    
    listen(serv_sock,1);
    printf("wait for connect...\n");

    clnt_addr_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_sz);
    printf("client connected...\n");

    while(1)
    {
        read_cnt = fread((void*)buf,1,BUF_SIZE,fp);
        if(read_cnt<BUF_SIZE)
        {
            write(clnt_sock,buf,read_cnt);
            break;
        }
        write(clnt_sock,buf,BUF_SIZE);
    }    

    //半关闭
    shutdown(clnt_sock,SHUT_WR);
    read(clnt_sock,buf,BUF_SIZE);
    printf("Message from client: %s\n",buf);
    
    close(clnt_sock);
    close(serv_sock);
    fclose(fp);

    return 0;
}

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}