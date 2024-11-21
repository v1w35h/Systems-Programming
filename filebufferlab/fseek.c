#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    FILE *file;
    // struct BufReader *br;
    
    char filename[] = "read.txt";
    char buffer[100];
    char ch;

    file = fopen("read.txt", "r");
    if (file == NULL) {
        perror("open");
        return 1;
    }

    off_t new_offset = fseek(file, 1, SEEK_CUR); 
    if (new_offset == -1) {
        perror("fseek");
        return 1;
    }

    printf("New offset: %ld\n", new_offset);

    fclose(file);
    return 0;
}