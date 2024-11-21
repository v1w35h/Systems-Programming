#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct{
    int a;
    int b;
    int c;
} data;

int main(){
    int fd;
    data *d;

    fd = shm_open("/myshared.mem", O_CREAT | O_EXCL | O_RDWR, 0666);
    if(fd < 0){
        perror("/myshared.mem");
        return -1;
    }

    ftruncate(fd,sizeof(*d));

    d = mmap(NULL, sizeof(*d), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(d == MAP_FAILED){
        perror("mmap");
        goto    shm_cleanup;
    }
    close(fd);

    d->a = -1;
    d->b =  100;
    d->c  = 1234;

    munmap(d, sizeof(*d));
shm_cleanup:
    sleep(4);
    shm_unlink("/myshared.mem");
}