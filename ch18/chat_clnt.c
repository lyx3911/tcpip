#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void * send_msg(void* arg);
void * recv_msg(void* arg);
void error_handling(char * msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_adr;

    if(argc!=4){
        printf("please enter serve_ip, port, and user name \n");
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0 ,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    sprintf(name, "[%s]", argv[3]);
    printf("user %s entered chatting room.\n", name);

    if( connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1 )
        error_handling("connect error!");

    pthread_t thr_send, thr_recv;
    void * thread_return;
    pthread_create(&thr_send, NULL, send_msg, (void*)&sock);
    pthread_create(&thr_recv, NULL, recv_msg, (void*)&sock);

    pthread_join(thr_send, &thread_return);
    pthread_join(thr_recv, &thread_return);

    close(sock);
    return 0;
}

void error_handling(char * msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

void * send_msg(void * arg)
{
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE+ BUF_SIZE];
    while(1)
    {
        fgets(msg, BUF_SIZE, stdin);
        if(!strcmp(msg,"Q\n")){
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s",name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}

void * recv_msg(void *arg)
{
    int sock = *((int*)arg);
    char msg[BUF_SIZE + NAME_SIZE];
    int msg_len;
    while(1)
    {
        msg_len = read(sock, msg, BUF_SIZE+NAME_SIZE);
        msg[msg_len] = 0;
        fputs(msg, stdout);
    }
    return NULL;
}