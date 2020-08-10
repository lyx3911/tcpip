#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(char *message);

int main()
{
    int tcp_sock, udp_sock;
    int sock_type;
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);
    tcp_sock = socket(PF_INET,SOCK_STREAM,0);
    udp_sock = socket(PF_INET,SOCK_DGRAM,0);

    printf("sOCK_STREAM: %d\n",SOCK_STREAM);
    printf("SOCK_DGRAM: %d\n",SOCK_DGRAM);


    //套接字选项
    state = getsockopt(tcp_sock,SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state==-1)
        error_handling("get sock opt error...");
    printf("socket type one: %d\n",sock_type);

    state = getsockopt(udp_sock,SOL_SOCKET,SO_TYPE, (void*)&sock_type, &optlen);
    if(state==-1)
        error_handling("get sock opt error...");
    printf("socket type two: %d\n",sock_type);


    //查看buf大小
    int send_buf, recv_buf;
    socklen_t len = sizeof(recv_buf);
    state = getsockopt(tcp_sock,SOL_SOCKET, SO_RCVBUF,(void*)&recv_buf, &len);
    if(state==-1)
        error_handling("get recv_buf size error...");
    printf("read buf size: %d\n",recv_buf);

    len = sizeof(send_buf);
    state = getsockopt(tcp_sock,SOL_SOCKET,SO_SNDBUF,(void*)&send_buf,&len);
    if(state==-1)
        error_handling("get send_buf size error...");
    printf("send buf size: %d\n",send_buf);

    //设置buf大小
    send_buf = 1024*3;
    recv_buf = 1024*3; //都设为3M
    state = setsockopt(tcp_sock,SOL_SOCKET,SO_RCVBUF,(void*)&recv_buf,sizeof(recv_buf));
    if(state==-1)
        error_handling("set recv_buf error...");
    len = sizeof(recv_buf);
    state = getsockopt(tcp_sock,SOL_SOCKET,SO_RCVBUF,(void*)&recv_buf,&len);
    if(state==-1)
        error_handling("get recv_buf size after reset error...");
    printf("after reset, recv buf size: %d\n",recv_buf);

    state = setsockopt(tcp_sock,SOL_SOCKET,SO_SNDBUF,(void*)&send_buf,sizeof(send_buf));
    if(state==-1)
        error_handling("set send_buf error...");
    len = sizeof(send_buf);
    state = getsockopt(tcp_sock,SOL_SOCKET,SO_SNDBUF,(void*)&send_buf,&len);
    if(state==-1)
        error_handling("get send_buf size after reset error...");
    printf("after reset, send buf size: %d\n",send_buf);

    return 0;
}

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}