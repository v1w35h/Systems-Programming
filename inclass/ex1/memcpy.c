#include <stdio.h>
#include <string.h>

int main(void){
    // int s[] = {1, 2, 3 ,4 ,5, 6,7,8,9,200};
    // int d[] = {1, 2, 3 ,4 ,5, 6,7,8,9,10};
    char s[]= "abcdefghijkl";
    char d[]= "abcdefgiijkl";

    printf("%d\n",memcmp(d, s, sizeof(char)*10));

    // printf("d: %d\n", d);
    // for(int i=0; i< 10; i++){
    //     printf("d[%d]: %d\n",i, s[i]);
        
    // }

    // char e[10];

    // memcpy(e,s+8, sizeof(char)*9);
    // e[9] = '\0';

    // printf("e: %s\n", e);

}