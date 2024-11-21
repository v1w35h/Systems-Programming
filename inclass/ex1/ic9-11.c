#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

// struct stat {
// st_dev; /* Device number */
// st_ino; /* Inode */
// st_mode; /* Type and permissions list */
// st_nlink; /* Number of hard links */
// st_uid; /* User ID (owner) */
// st_gid; /* Group ID (owner) */
// st_size; /* Number of bytes */
// };


int main(int argc, char *argv[]) {
    // int fd;
    // ssize_t ret;
    // char buf[256];
    // char mybyte;

    // fd = open("john.txt",O_RDONLY, 0666);
    // if(fd < 0) {
    //     // printf("%d\n",errno);
    //     perror("john.txt");
    //     return -1;
    // }
    // lseek(fd, 5, SEEK_SET);
    // read(fd, buf, 5);
    // printf("%lu: %.5s\n", lseek(fd, 0,SEEK_CUR), buf);
    // read(fd, &mybyte,1);
    // printf("Character = '%c' (%d)\n", mybyte, mybyte);
    // close(fd);

    int ret;

    struct stat st;

    ret = stat("john.txt",&st);
    if(ret < 0){
        perror("stat");
        return -1;
    }
    printf("%lu\n", st.st_mode);
}