#include "pagealloc.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){
    page_init(16);
    printf("------------------------------------\n");

    printf("pages taken: %lu\n", pages_taken());
    printf("pages free: %lu\n", pages_free());  


    void* a = page_alloc(3);
    printf("pages taken: %lu\n", pages_taken());
    // page_free(c);
    printf("pages free: %lu\n", pages_free());  

    page_free(a);
    printf("after free a--------\n");

    printf("pages taken: %lu\n", pages_taken());
    // page_free(a);
    printf("pages free: %lu\n", pages_free());  

    void* b = page_alloc(2);
    printf("pages taken: %lu\n", pages_taken());
    // page_free(a);
    printf("pages free: %lu\n", pages_free());  

    page_free(b);
    printf("after free b--------\n");

    printf("pages taken: %lu\n", pages_taken());
    // page_free(a);
    printf("pages free: %lu\n", pages_free());  




    // if(page_alloc(5) == NULL){
    //     printf("Can't alloc\n");
    // }
    // else{
    //     printf("Can alloc\n");
    // }
    // if(page_alloc(5) == NULL){
    //     printf("Can't alloc\n");
    // }
    // else{
    //     printf("Can alloc\n");
    // }
    printf("pages taken: %lu\n", pages_taken());
    // // page_free(b);
    printf("pages free: %lu\n", pages_free());

    page_deinit();
}