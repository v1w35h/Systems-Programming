#include <stdio.h>
#include <stdlib.h>
#include "bufread.h"
#include <fcntl.h>
#include <unistd.h>

int main() {
    // FILE *file;
    int file;
    
    // char filename[] = "bee_movie_script.txt";
    char filename[] = "read.txt";
    char buffer[100];
    char ch;

    // 1. Using fopen to open a file in read mode
    file = open(filename, O_RDONLY, "r");
    if (file <0) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }

    // off_t seek = lseek(file, -5, SEEK_END);
    // printf("%lu\n", seek);
    
    // off_t tell = lseek(file, 0, SEEK_CUR);
    // printf("%lu\n", tell);


    // 2. Using fgets to read a line from the file
    printf("\nReading file using fread (block read):\n");
    size_t bytesRead = read(file, buffer, sizeof(char));
    buffer[bytesRead] = '\0';  // Null-terminate the buffer
    printf("%s\n", buffer);

    printf("Reading a line from the file using fgets:\n");
    if (gets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    } else {
        printf("Error: Failed to read a line from the file.\n");
    }
    

    // 3. Using fgetc to read characters one by one
    // printf("\nReading characters from the file using fgetc:\n");
    // while ((ch = fgetc(file)) != EOF) {
    //     putchar(ch);
    //     // printf("%c",ch);
    // }
    // printf("\n");


    // Rewind file pointer to the beginning of the file
    // rewind(file);

    // 4. Using fread to read a block of data from the file

    // 5. Close the file
    close(file);

    return 0;
}