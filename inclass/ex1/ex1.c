#include <stdio.h>

int printf(const char *format, ...);

int main(int argc, char *argv[]){
    printf("I'm running program %s\n", argv[0]);
}