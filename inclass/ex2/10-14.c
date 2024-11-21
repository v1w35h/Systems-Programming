#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void *worker(void * arg);

int main() {
    pthread_t tid;

    int myvar = -1234;
    int *val;

    printf("Before Create\n");
    if(pthread_create(&tid, NULL, worker, &myvar)){
        perror("pthread_create");
        return -1;
    }
    printf("After Create\n");
    pthread_join(tid, &val);
    printf("After join: %d: %d\n", myvar, (int)val);      //just like wait
    return 0;
}

static void *worker(void *arg) {
    printf("%d\n", *((int*)arg));
    // *((int*)arg) =1789;
    // exit(EXIT_FAILURE);         // exit kills the process, use pthread_exit instead
    pthread_exit((void*)912345);
    // return (void*)9999999;
}

