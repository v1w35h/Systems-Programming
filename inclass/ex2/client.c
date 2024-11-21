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

    fd = shm_open("/myshared.mem", O_RDONLY, 0);
    if(fd < 0){
        perror("/myshared.mem");
        return -1;
    }

    ftruncate(fd,sizeof(*d));

    d = mmap(NULL, sizeof(*d), PROT_READ, MAP_SHARED, fd, 0);
    if(d == MAP_FAILED){
        perror("mmap");
        return -1;
    }
    close(fd);
    printf("%d %d %d\n", d->a,d->b,d->c);
    munmap(d,sizeof(*d));
}