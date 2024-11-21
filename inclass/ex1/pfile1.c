#include <stdio.h>
#include <fcntl.h> // open(), #defines
#include <unistd.h> // read(), write()

struct MyStruct1
{
    char y;
    int s;
    int jl;
    double u;
    char m;
    char n;
    char x;
    char p;
    char gayn;

    int *l;
    char gaypeytom;

//     /* data */
};


int main(int argc, char *argv[])
{
    short*  ptr = (short*)1000;
    printf("%hu\n", &ptr[10]);
    // int fd;
    // ssize_t bytes;
    // char buffer[64];
    struct MyStruct1 ms;
 

    printf("%d\n", sizeof(struct MyStruct1));

    // if (argc < 2) {
    //     printf("Usage: %s <file name>\n", argv[0]);
    //     return -1;
    // }

    // fd = open(argv[1], O_RDONLY);
    // if (fd < 0) {
    //     perror("open");
    //     return -1;
    // }

    // bytes = read(fd, buffer, 63);
    // if (bytes < 0) {
    //     perror("read");
    //     close(fd);
    //     return -1;
    // }
    // close(fd);

    // printf("Read: '%.*s'\n", (int)bytes, buffer);

    return 0;
}
