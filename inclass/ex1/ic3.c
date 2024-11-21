#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mystrlen(char str[]){
    int len;
    for(len = 0; str[len] != '\0'; len+= 1);
    return len;
}

int main(){
    char str[]  = "Hello";
    char str2[] = "Pelmz";

    int r = memcmp(str,str2,5);
    printf("%d\n", r);
    char input[255];

    // do{
    //     printf("Enter value ('done' to quit): ");
    //     scanf("%s", input);

    //     // if(input == "done"){
    //     //     break;
    //     // } will not work == does not work
    //     if(!strcmp(input, "done")){
    //         break;
    //     }
    // }while(1);
    char str3[3];
    char str4[]  = {1,2,3,4,5};
    // char str2[3]; //produces logic error when copied
    strcpy(str3, str4);

    sprintf(str3, "Hello World");
    // snprintf(str3,5,"Hello World\n");
    // strncpy(str3,"Hello World", 2);
    // str3[2] = '\0';

    // strncpy(str4,"Jo",5);
    // not c-style string (not ending in \0)
    printf("%s\n", str4);
    printf("%d\n", mystrlen(str));

    for(int i =0; i< sizeof(str); i+=1){
        printf("%d: %-3u %c\n", i, str3[i], str3[i]);
    }

}