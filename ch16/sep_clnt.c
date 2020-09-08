#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main( int argc, char * argv[])
{
    int sock;
    FILE *read_fp;
    FILE *write_fp;

    struct sockaddr_in serv_adr;
    char buf[BUF_SIZE] = {0,};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    connect(sock,(struct sockaddr*)&serv_adr, sizeof(serv_adr));

    read_fp = fdopen(sock, "r");
    write_fp = fdopen(sock, "w");

    while(1){
        if(fgets(buf, BUF_SIZE, read_fp)==NULL) break;

        fputs(buf,stdout);
        fflush(stdout);
    }

    fputs("FROM CLIENT: Thank you! \n",write_fp);
    fflush(write_fp);
    fclose(write_fp);
    fclose(read_fp);

    return 0;
}