#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char* argv[])
{
    struct sockaddr_in mul_adr;
    int str_len, state;

    int time_live = TTL;
    char buf[BUF_SIZE] = "hello world";

    int send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(atoi(argv[2])); 

    setsockopt(send_sock,IPPROTO_IP, IP_MULTICAST_TTL,(void*)&time_live, sizeof(time_live));

    // while(1) //broadcast
    // {
        sendto(send_sock, buf, sizeof(buf), 0, (struct sockaddr*)&mul_adr, sizeof(mul_adr));
        sleep(2);
    // }
    
    close(send_sock);

    return 0;
}

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}