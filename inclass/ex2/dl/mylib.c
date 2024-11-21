#include <stdio.h>

int funky_func(int i, int j, int k){
    printf("Hello world %d %d %d\n", i,j,k);
    return i+j+k;
}

int (*myfunctorun)(int i, int j, int k) = funky_func;