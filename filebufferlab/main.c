#include <stdio.h>
#include <stdlib.h>
#include "bufread.h"

int main() {
    struct BufReader *br;
    
    char filename[] = "bee_movie_script.txt";
    char buffer[100];
    // char ch;
    // void *dest;
    char buf[256];

    char line[15];



    br = br_open(filename, 64, 20);
    br_seek(br, 5, SEEK_SET);

    size_t tell = br_tell(br);

    printf("%ld\n", tell);
    // br_seek(br, 5, SEEK_CUR);
    // br_seek(br, -5, SEEK_END);

    size_t read = br_read(br, buf, 4);
    printf("%ld\n", read);
    printf("%s\n", buf);



    // while(br_getline(br,line,sizeof(line))){
    //     printf("%s\n", line);
    // }
    // int c = br_getchar(br);
    // printf("%c\n", (char)c);
    // br_getline(br,line,2);
    // printf("%s\n", line);


    // size_t tell = br_tell(br);

    // printf("%ld\n", tell);

    br_close(br);

    return 0;
}