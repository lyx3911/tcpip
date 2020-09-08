#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main( int argc, char * argv[])
{
    int serv_sock, clnt_sock;
    FILE *read_fp;
    FILE *write_fp;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz = sizeof(clnt_adr);

    char buf[BUF_SIZE] = {0,};

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
    listen(serv_sock,5);

    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_sock, &clnt_adr_sz);
    read_fp = fdopen(clnt_sock, "r");

    int clnt_sock2 = dup(clnt_sock);
    write_fp = fdopen(clnt_sock2, "w");

    fputs("FROM SERVER: Hi~ client? \n",write_fp);
    fputs("I love all of the world \n", write_fp);
    fputs("You are awesome! \n", write_fp);
    fflush(write_fp);

    shutdown(fileno(write_fp),SHUT_WR); //传递EOF
    fclose(write_fp);

    fgets(buf,sizeof(buf),read_fp);
    fputs(buf,stdout);
    fclose(read_fp);

    return 0;
}