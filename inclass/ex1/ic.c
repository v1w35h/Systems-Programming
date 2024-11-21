#include <stdio.h> //input output
#include <stdlib.h> //standard library

int main(int argc, char*argv[])
{
    char mystring[] = "Hello World"; //array does not have length. takes up one more byte
    // char mystring[] = {'a','b','c'};

    // double i = 1.5F;
    unsigned long i = -1;

    float j = 1.7777777;

    // printf("%d\n", (int)j); // typecast done first
    printf("%.2f\n", j);

    // printf("%f\n", i);
    printf("%-33s '%7d'\n", mystring, i);
    // printf("%s\n", mystring);
    // printf("%lf\n", i);
    // %d - decimal (signed 32 bit integer) base 10
    // %u - unsigned 32 bit integer
    // %lu - long unsigned int 64 bit
    // %ld - long decimal (base 10) 64 bit
}

