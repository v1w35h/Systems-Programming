#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum {
    WS_IDLE = 0,
    WS_WORK,
    WS_DIE
} WorkerState;

typedef struct {
    pthread_t tids;
    WorkerState state;
    unsigned int val;
    int has_stick;
    unsigned int myid;
} stick;

// typedef struct{
//     stick    **buffer;
//     unsigned int size;
//     unsigned int at;
//     unsigned int capacity;
//     bool     done_talking;
//     pthread_mutex_t mutex;
//     // pthread_cond_t not_done_talking;
//     // pthread_cond_t not_talking;
//     // pthread_cond_t done_talking;
// } Queue;

void *worker(void *arg);

int main(){
    // Queue q;
    // pthread_t tid[5];
    int num_threads = 5;
    stick *s;
    s = calloc(num_threads, sizeof(s));

    // q.capacity = num_threads;
    // q.size = 0;
    // q.at = 0;
    // q.buffer = calloc(num_threads, sizeof(stick *));
    // q.done_talking = false;
    int i;
    // , ids[num_threads];
    // void *retval;

    for(i = 0; i < num_threads; i++){
        s[i].myid =i;
        s[i].state = WS_IDLE;
        if(pthread_create(&s[i].tids, NULL, worker, s+i) !=0){
            perror("pthread_create");
            exit(1);
        }
    }

    int start =0;
    int end = 10;

    while (start <= end) {
        for(i=0; i <num_threads; i+=1) {
            if(s[i].state == WS_IDLE) {
                start++;
                s[i].state = WS_WORK;
            }
        }
    }

    for(i = 0; i < num_threads; i++){
    //     printf("%d is trying to pass the stick\n", i);
    //    if(pthread_join(tid[i],&retval)!= 0){
    //     perror("join"); 
    //     exit(1);
    //    }
    //    printf("%d is passing the stick %d\n", i);
        while(s[i].state != WS_IDLE);
        s[i].state = WS_DIE;
        pthread_join(s[i].tids, NULL);
    }

    free(s);

    return 0;
}

void *worker(void *arg){
    stick *s = (stick *)arg;
    unsigned int i;
    for (;;) {
        while (s->state == WS_IDLE){
            usleep(1000);
        }
        if(s->state == WS_DIE){
            break;
        }

        s->has_stick = true;

        printf("thread %d %s\n", s->myid, s->has_stick ? "has the stick" : "does not have the stick");
        s->state = WS_IDLE;
    }
    return NULL;
}

// #include <pthread.h> 
// #include <stdio.h> 
// #include <stdlib.h> 
// #include <string.h> 
// #include <unistd.h> 
  
// pthread_t tid[5]; 
// int counter; 
  
// void* trythis(void* arg) 
// { 
//     unsigned long i = 0; 
//     counter += 1; 
//     printf("\n Job %d has started\n", counter); 
  
//     for (i = 0; i < (0xFFFFFFFF); i++) 
//         ; 
//     printf("\n Job %d has finished\n", counter); 
  
//     return NULL; 
// } 
  
// int main(void) 
// { 
//     int i = 0; 
//     int error; 
  
//     while (i < 5) { 
//         error = pthread_create(&(tid[i]), NULL, &trythis, NULL); 
//         if (error != 0) 
//             printf("\nThread can't be created : [%s]", strerror(error)); 
//         i++; 
//     } 
  
//     // pthread_join(tid[0], NULL); 
//     // pthread_join(tid[1], NULL); 

//     while (i < 5) { 
//         error = pthread_join((tid[i]), NULL); 
//         if (error != 0) 
//             printf("\nThread can't be joined : [%s]", strerror(error)); 
//         i++; 
//     } 
  
//     return 0; 
// } 