#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
    int fd;
    ssize_t ret;
    char buf[256];

    fd = open("john.txt", O_RDONLY);
    if(fd < 0) {
        printf("%d\n", errno);
        perror("john.txt");
        return -1;
    }
    // write(fd, "Hello World\n", strlen("Hello World\n"));
    ret = read(fd, buf, 5);
    if(ret < 0){
        perror("read");
        return -1;
    }
    buf[ret] = '\0';
    ret = read(fd, buf, 5);
    if(ret < 0){
        perror("read");
        return -1;
    }
    printf("%lu: %s\n",ret, buf);  // does not know where null terminator is at
    off_t val = lseek(fd,0,SEEK_CUR);
    printf("We are %ld bytes into the file.\n", val);
    close(fd);
}