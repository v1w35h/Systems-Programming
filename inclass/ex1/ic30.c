#include <stdio.h>
#include <string.h>
#include <stdlib.h> // malloc, calloc, free\

struct MyStruct {
    int i;
    double j;
    float k;
};

typedef struct MyStruct MS;

int main(void){
    MS *ms;

    ms = calloc(10, sizeof(struct MyStruct));

    // 3
    (ms + 3)->i = 100;

    free(ms);
}