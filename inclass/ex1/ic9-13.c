#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

// // prototype
static void print_cl(mode_t);

int main() {
    struct stat st;
    DIR *dir;
    struct dirent *DENT;


    dir  = opendir(".");
    if(!dir){
        perror("opendir");
        return -1;
    }
    printf("hello world\n");

    while((DENT = readdir(dir)) !=  NULL) {
        if (strcmp(DENT->d_name, ".") && strcmp(DENT->d_name, "..")){
            stat(DENT->d_name, &st);
            print_cl(st.st_mode);
            printf("    %-10lu: %s\n", st.st_size, DENT->d_name);
        }
    }


    closedir(dir);
}

static void print_cl(mode_t mode){
    if (S_ISDIR(mode)){
        putchar('d');
    }
    else if (S_ISLNK(mode)){
        putchar('l');
    }
    else{
        putchar('-');
    }

    for (int i = 6; i>= 0; i-=3){
        putchar(mode & (i << (i+2)) ? 'r' : '-');
        putchar(mode & (i << (i+1)) ? 'w' : '-');
        putchar(mode & (i << i) ? 'x' : '-');

    }
}

//  links and unlink

// int main(){
//     link("ic.c", "you.c");
//     unlink("you.c");
// }