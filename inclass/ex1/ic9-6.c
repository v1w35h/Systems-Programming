#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc,  char *argv[])  {
    void *ptr;

    ptr = mmap(NULL, 4096,  PROT_READ | PROT_WRITE, MAP_PRIVATE , -1, 0);
    if(ptr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    int *iptr = (int *)ptr;
    iptr[0] = 1234;
    iptr[1] = 5678;

    printf("%p %d %d", &iptr, iptr[0], iptr[1]);
    munmap(ptr,4096);
}



//p1 

// int main(int argc, char *argv[]){
//     int fd;
//     char *ptr;
//     char buf[50];
//     int size;

//     fd = open("text.txt", O_RDWR);
//     if (fd < 0){
//         perror("ic.cpp");
//         return -1;
//     }
//     read(fd, buf, 10);
//     write(1,buf,10);
//     size = lseek(fd, 0, SEEK_END);
//     printf("%d\n", size);
//     lseek(fd,10*3,SEEK_SET);
//     read(fd,buf,10);

    


//     ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//     if(ptr == MAP_FAILED){
//         perror("mmap");
//         return -1;
//     }
//     close(fd);

//     // ptr[0] = 'V';
//     // ptr[1] = 'Z';

//     // for(int i = 0;  i < size; i++){
//     //     putchar(ptr[i]);
//     // }
//     // putchar('\n');

//     munmap(ptr, size);
// }