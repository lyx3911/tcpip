#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void * request_handler(void *arg);
void send_data(FILE* fp, char* ct, char* filename);
char* content_type(char* file);
void send_error(FILE *fp);
void error_handling(char* message);

int main(int argc, char* argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_adr_size;

    char buf[BUF_SIZE];
    pthread_t t_id;
    if(argc!=2){
        printf("please enter port info...");
        exit(1);
    }

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind error.");
    
    if(listen(serv_sock, 20)==-1)
        error_handling("listen error.");

    
    while(1)
    {
        clnt_adr_size = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_size);

        printf("Connection request: %s %d \n", inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));

        pthread_create(&t_id, NULL, request_handler, &clnt_sock);
        pthread_detach(t_id);
    }
    close(serv_sock);
    return 0;
}

void * request_handler(void *arg)
{
    int clnt_sock = *((int*)arg);
    char req_line[SMALL_BUF];

    FILE* clnt_read;
    FILE* clnt_write;

    char method[10];
    char ct[15];
    char filename[30];

    clnt_read = fdopen(clnt_sock,"r");
    clnt_write = fdopen(dup(clnt_sock), "w"); //要复制

    fgets(req_line, SMALL_BUF, clnt_read);

    if(strstr(req_line, "HTTP/")==NULL)
    {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return;
    }  

    strcpy(method, strtok(req_line," /"));
    strcpy(filename, strtok(NULL, " /"));
    strcpy(ct, content_type(filename));

    if(strcmp(method,"GET")!=0)
    {
        send_error(clnt_write);
        fclose(clnt_write);
        fclose(clnt_read);
        return;
    }

    fclose(clnt_read);
    send_data(clnt_write, ct, filename);  
}

void send_data(FILE *fp, char *ct, char* file_name)
{
    char protocol[] = "HTTP/1.0 200, OK\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Conent-length:2048\r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE * send_file;

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    send_file = fopen(file_name, "r");

    if(send_file==NULL)
    {
        fputs("file not exist\n", stderr);
        send_error(fp);
        return ;
    }

    //传输头信息
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    //传输请求数据
    while(fgets(buf, BUF_SIZE, send_file)!=NULL){
        fputs(buf, fp);
        fflush(fp);
    }
    fflush(fp);
    fclose(fp);
}

char* content_type(char *file)
{
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];

    strcpy(file_name,file);
    strtok(file_name, ".");
    strcpy(extension, strtok(NULL,"."));

    if(!strcmp(extension,"html")||!strcmp(extension,"htm"))
        return "text/html";
    else  return "text/plain";

}

void send_error(FILE * fp)
{
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char clnt_len[] = "Content-length:2048\r\n";
    char clnt_type[] = "content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head><body>error, please check your filename and connect type.</body></html>";

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(clnt_len, fp);
    fputs(clnt_type, fp);
    fputs(content, fp);
    fflush(fp);
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}