#include <stdio.h>
#include <fcntl.h>

int main(void)
{
    FILE *fp;
    int fd = open("data.dat",O_WRONLY|O_CREAT|O_TRUNC);

    if(fd == -1) {
        printf("file open error! \n");
        return -1;
    }

    fp = fdopen(fd, "w");
    fputs("Network C programming \n",fp);
    fclose(fp);
    return 0;
}