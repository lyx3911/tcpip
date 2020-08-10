#include <stdio.h>
#include <stdlib.h>
// #include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(char *message);

int main()
{
    struct hostent *host;
    char *ip = "www.bing.com";
    host = gethostbyname(ip);

    if(!host) 
        error_handling("get host... error");
    
    printf("Offical name: %s\n",host->h_name);
    for(int i=0;host->h_aliases[i];i++)
        printf("Aliases %d: %s\n",i+1,host->h_aliases[i]);
    for(int i=0;host->h_addr_list[i];i++)
        printf("IP addr %d: %s\n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i])); //注意要使用inet函数转换
}

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}