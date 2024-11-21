#include <stdio.h>
#include <string.h>

static char *myfunc(char *a){
    size_t i;
    i = strcspn(a," ");
    a[i] = '\0';

    return a;
}

struct MyStruct1{
    int i,j,k;
};

struct MyStruct2{
    int l,i,j,k;
};

int main(void) {
    //p1
    // char a[] = "Hello World";
    // char *ptr;
    // ptr = myfunc(a);

    // printf("%s\n", ptr);


    //p2
    // size_t i;

    // i = strcspn("Hello World", " ");

    // printf("%lu \n", i);


    //p3
    // char a[] = "Hello World";
    // char *ptr;
    // ptr = myfunc(a);

    // printf("%s\n", a);


    //p4 
    // const struct MyStruct1 ms1 = {10,20,30};
    // struct MyStruct2 ms2;
    // char *ptr = (char*)&ms2;

    // ms2.i = ms1.i;
    // ms2.j = ms1.j;
    // ms2.k = ms1.k;
    // // memcpy(ptr+ 4, &ms1, sizeof(ms2) - 4);
    // memmove(ptr, ptr+4, sizeof(ms2) - 4);


    // printf("%d %d %d\n", ms2.i, ms2.j, ms2.k);

    // memory regions are overlapping
    // gives effect of allowing you to copy overlapping memory regions

    //p5
    char a[] = "Hello World";
    // char b[] = "hi hi";

    memset(a, 'b', 5);

    printf("%s\n",a);

}