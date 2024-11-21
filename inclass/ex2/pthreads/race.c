// // program forks 2 threads which share an integer, on which there is a race condition
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

// // this is information shared between the two threads
// typedef struct{
//     int i;
//     int die;
// } Shared_info;

// // this is info unique to each thread, (SI is a pointer to shared data)
// typedef struct{
//     int id;
//     Shared_info *SI;
// } Info;

// // thread code {pretty simple :/}
// void *worker(void *arg) {
//     Info *I;

//     I = (Info *)arg;

//     while (!I->SI->die){
//         I->SI->i = I->id;
//     }
//     return NULL;
// }

// // Main code sets up the shared and unique info, then forks off two threads. It then sleeps for two seconds and prints out the shared variable, si.i. 
// //Finally, it calls pthread_join() to wait for the two threads to die, and prints out the shared variable again

// int main(int argc, char **argv){
//     pthread_t tids[2];
//     Shared_info si;
//     Info I[2];
//     void *retval;

//     I[0].id = 0;
//     I[0].SI = &si;

//     I[1].id = 1;
//     I[1].SI = &si;

//     si.die = 0;

//     if(pthread_create(tids, NULL, worker, I) != 0){
//         perror("pthread_create");
//         exit(1);
//     }
//     if(pthread_create(tids+1, NULL, worker, I+1) != 0){
//         perror("pthread_create");
//         exit(1);
//     }

//     si.die = 1;

//     printf("%d\n", si.i);

//     if(pthread_join(tids[0], &retval) != 0){
//         perror("pthread_join");
//         exit(1);
//     }
//     if(pthread_join(tids[1], &retval) != 0){
//         perror("pthread_join");
//         exit(1);
//     }
//     printf("%d\n", si.i);

//     return 0;
// }

typedef struct {
    int id;
    int size;
    int iterations;
    char *s;
} Thread_struct;

void *worker(void *arg){
    int i,j;
    Thread_struct *t;

    t = (Thread_struct*)arg;

    for(i = 0; i < t->iterations; i++) {
        for ( j = 0; j < t->size-1; j++)  t->s[j] = 'A'+ t->id;
        t->s[j] = '\0';
        printf("Thread %d: %s\n", t->id, t->s);
    }
    return NULL;
}

int main(int argc, char **argv){
    pthread_t *tid;
    Thread_struct *t;
    void *retval;
    int nthreads, size, iterations, i;
    char *s;

    if(argc != 4) {
        fprintf(stderr, "usage: race nthreads stringsize iterations\n");
        exit(1);
    }
    if(1 != sscanf(argv[1], "%d", &nthreads)){
        printf("incorrect nthreads\n");
        return -1;
    }
    if(1 != sscanf(argv[2], "%d", &size)){
        printf("incorrect size\n");
        return -1;
    }
    if(1 != sscanf(argv[3], "%d", &iterations)){
        printf("incorrect iterations\n");
        return -1;
    }

    tid = (pthread_t *)malloc(sizeof(pthread_t) *nthreads);
    t = (Thread_struct *)malloc(sizeof(Thread_struct) *nthreads);
    s = (char*) malloc(sizeof(char *) *size);

    for( i =0; i < nthreads; i++){
        t[i].id = i;
        t[i].size = size;
        t[i].iterations = iterations;
        t[i].s = s;
        if(pthread_create(tid+i, NULL, worker, t+i) != 0){
            perror("pthread_create");
            return -1;
        }
    }

    for( i =0 ; i < nthreads; i++) {
        if(pthread_join(tid[i], &retval) != 0){
            perror("pthread_join");
            return -1;
        }
    }

    free(tid);
    free(t);
    free(s);
    return 0;
}