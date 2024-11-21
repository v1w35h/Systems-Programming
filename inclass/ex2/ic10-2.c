#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define NUM_ITERATIONS  1000000

void child() {
    int *i;
    int fd;
    int iterations;

    sleep(2);
    fd = shm_open("/myshared", O_RDWR, 0);
    i = mmap(NULL, sizeof(*i), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (i == MAP_FAILED){
        perror("mmap_client");
        return;
    }
    for(iterations = 0; iterations < NUM_ITERATIONS; iterations+=1){
        *i  =  *i + 1;
    }
    munmap(i,sizeof(*i));

}

void parent() {
    int iterations;
    int *i;
    int fd;
    fd = shm_open("/myshared", O_CREAT | O_RDWR | O_EXCL, 0666);
    ftruncate(fd, sizeof(*i));
    i = mmap(NULL, sizeof(*i), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if(i == MAP_FAILED) {
        perror("mmap");
    }
    *i =  0;
    for(iterations = 0; iterations < NUM_ITERATIONS; iterations+=1){
        *i  =  *i + 1;
    }
    sleep(5);
    printf("i  = %d\n", *i);
    munmap(i,sizeof(*i));
    shm_unlink("/myshared");
}


int main(void) {
    pid_t pid;

    int fd;

    pid = fork();
    if(pid == 0) {
        //child
        child();
    }
    else if (pid >  0) {
        parent();

    }
}