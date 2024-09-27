#include <stdio.h>
#include <stdlib.h>
#include "bufread.h"

int main() {
    FILE *file;
    struct BufReader *br;
    
    char filename[] = "read.txt";
    char buffer[100];
    char ch;

    // 1. Using fopen to open a file in read mode
    br = br_open(filename, 255, 20);
    // if()
    // file = fopen(filename, "r");
    // if (file == NULL) {
    //     printf("Error: Could not open file %s\n", filename);
    //     return 1;
    // }

    // // 2. Using fgets to read a line from the file
    // printf("Reading a line from the file using fgets:\n");
    // if (fgets(buffer, sizeof(buffer), file) != NULL) {
    //     printf("%s", buffer);
    // } else {
    //     printf("Error: Failed to read a line from the file.\n");
    // }

    // // 3. Using fgetc to read characters one by one
    // printf("\nReading characters from the file using fgetc:\n");
    // while ((ch = fgetc(file)) != EOF) {
    //     putchar(ch);
    //     printf("%c",ch);
    // }
    // printf("\n");

    // // Rewind file pointer to the beginning of the file
    // rewind(file);

    // // 4. Using fread to read a block of data from the file
    // printf("\nReading file using fread (block read):\n");
    // size_t bytesRead = fread(buffer, sizeof(char), sizeof(buffer) - 1, file);
    // buffer[bytesRead] = '\0';  // Null-terminate the buffer
    // printf("%s\n", buffer);

    // // 5. Close the file
    // fclose(file);
    br_close(br);

    return 0;
}