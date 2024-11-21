#include <stdio.h>

struct point{ int x; int y;};

void change_x(struct point *p){
    p->x = 3;
}

int main(int argc, char **argv){

    struct point p;
    p.x = 15;
    change_x(&p);
    printf("%d\n",p.x);

    long i =35;
    double d = 33.1415;
    int width = -10;
    int precision = 2;
    char str[] = "hello world";

    printf("%*s text after\n",width, str);
}