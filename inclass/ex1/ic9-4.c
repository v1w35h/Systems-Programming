#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

// struct Page {
//     union{
//         char c[4096];
//         int i[4096/4];
//         long l[4096/8];
//     };
// };


// int main(int argc, char *argv[]){
//     // int i, j;

//     // sscanf(argv[1], "%d", &i);

//     // for(j = 0; j < i; j++){
//     //     malloc(4);
//     // }

//     void *ptr;
//     struct  Page *p;

//     ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

//     if(ptr == MAP_FAILED) {
// // will not be able to  do  if(!ptr){}
//         perror("mmap");
//         return -1;
//     }

//     p = (struct Page *)ptr;

//     printf("%p %p", p, ptr);

//     p->i[0] = 100;

//     munmap(ptr,4096);
// }

// First half of class
//p2 
void *a(void);
void *b(void *ptr);
void *c(void *ptr);

struct MyStruct{
    int i;
    int j;
};

struct MyStruct2{
    struct MyStruct ms;
    int k;
};

struct List{
    int value;
    struct List *next;
};

int main(void) {
    //p1
//     int i;
//     do
//     {
//         int  i;
//         goto here;        //cant use these *points deducted
//     } while(0);
// here:

// p2
    void *ptr = a();
    if(ptr == NULL){
        return 0;
    }
    void *ptr2 = b(ptr);
    if(ptr2 == NULL){
        free(ptr);
        return -1;
    }
    void *ptr3 = c(ptr);
    if(ptr3 == NULL){
        free(ptr);
        free(ptr2);
        return -1;
    }
    free(ptr3);
    free(ptr2);
    free(ptr);

    //p3 
    // struct List ls;


}
