#include <stdio.h>
#include <string.h>
#include <stdlib.h> // malloc, calloc, free

static mycalloc(int nmemb, int size){
    return memset(malloc(nmemb *size),  0, nmemb* size);
}

struct MyStruct {
    int i;
    double j;
    float k;
};


int main(void){
    //p1
    // // new(malloc) needs delete(free) 
    // int *i = malloc(100000* sizeof(int))
    // int *i = calloc(100000, sizeof(int)); // clears out data 

    // // i[3] = 100;

    // // *(i +3) = 100;
    // // allocated memory
    // // *i = 100;

    // free(i);

    //p2
    // int **ptr;
    // // int* needs 8 bytes 
    // ptr = (int **)calloc(10, sizeof(int*));

    // // *ptr[0] = 100;
    // ptr[0][0] = 100; //will not work
}