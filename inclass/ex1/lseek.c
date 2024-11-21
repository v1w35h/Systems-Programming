#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main()
{
    int fd;
    char buff[10];
    fd=open("text.txt",O_RDONLY);\

    if(fd != -1){
        int seek =  lseek(fd, -10*2, SEEK_END);

        if(seek !=  -1){
            ssize_t numRead = read(fd, buff, 10);
            printf("%s",  buff);
        } 
    }

}