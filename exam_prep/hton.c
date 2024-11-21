#include <stdio.h>
#include <arpa/inet.h>

int main(){
    short i = 264;
    printf("%hu\n", htons(i));
}