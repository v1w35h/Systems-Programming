#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *printme(void *ip){
    int *i;

    i = (int*) ip;
    printf("Hi, I'm thread %d\n", *i);
    return NULL;
}

// static void *worker_1(void *arg) {
//     pthread_mutex_t *mtx = (pthread_mutex_t *)arg;
//     fprintf(stderr, "A");
//     pthread_mutex_unlock(mtx);
//     return NULL;
// }
// static void *worker_2(void *arg) {
//     pthread_mutex_t *mtx = (pthread_mutex_t *)arg;
//     pthread_mutex_lock(mtx);
//     fprintf(stderr, "B");
//     return NULL;
// }
// static void *worker_3(void *arg) {
//     pthread_mutex_t *mtx = (pthread_mutex_t *)arg;
//     pthread_mutex_lock(mtx);
//     fprintf(stderr, "C");
//     return NULL;
// }
int main() 
{
    // pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    // pthread_t tids[2];

    // pthread_mutex_lock(&mtx);
    // pthread_create(tids + 1, NULL, worker_2, &mtx);
    // pthread_create(tids + 0, NULL, worker_1, &mtx);
    // pthread_join(tids[0], NULL);
    // pthread_join(tids[1], NULL);
    // pthread_mutex_unlock(&mtx);
    // pthread_create(tids + 0, NULL, worker_3, &mtx);
    // pthread_join(tids[0], NULL);

    // pthread_mutex_destroy(&mtx);

    int i, ids[4];
    pthread_t tid[4];
    void *retval;

    for(i=0; i< 4; i++){
        ids[i] = i;
        if(pthread_create(tid+i, NULL, printme, ids+i) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    // if(pthread_join(tid, &status) != 0) {
    //     perror("pthread_join");
    //     exit(1);
    // }
    for (i = 0; i < 4; i++) {
        printf("Trying to join with thread %d\n", i);
        if (pthread_join(tid[i], &retval) != 0) { perror("join"); exit(1); }
        printf("Joined with thread %d\n", i);
    }

    return 0;
}