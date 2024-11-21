#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

bool loopit = true;

static void sighandle(int sig) {
    if(sig == SIGINT){
        loopit = false;
    }
    else if(sig == SIGSEGV){
        printf("Segmentation Fault :)\n");
    }
}

int main(int argc, char *argv[]){
//-----------------------------------------------------------------------------

    // signal(SIGINT, sighandle);

    // do{
    //     printf("Yo mama.. he he he\n");
    //     usleep(10000);
    // } while(loopit);
    // printf("Okay, I'll stop insulting yo mama\n");

//-----------------------------------------------------------------------------

    // int *i = NULL;
    // signal(SIGSEGV, SIG_DFL);

    // *i = 12345;
    // printf("Okay, I'll stop insulting yo mama: %p\n", *i);
    // printf("Hello World\n");

//-----------------------------------------------------------------------------

    unsigned long i = 0;
    signal(SIGINT, sighandle);

    do{
        i++;
    } while(loopit);
    printf("Went through %lu iterations.\n", i);

//-----------------------------------------------------------------------------

    return 0;
}