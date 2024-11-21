#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

typedef enum {
    WS_IDLE = 0,
    WS_WORK,
    WS_DIE
} WorkerState;

typedef struct{
    pthread_t tid;
    WorkerState state;
    unsigned int value;
    int is_prime;
    unsigned int myid;
} Worker;

static void *worker(void *arg);

int main(int argc, char *argv[]){
    Worker *workers;
    unsigned int num_threads;
    unsigned int start_value;
    unsigned int end_value;
    unsigned int i;
    if(argc != 4){
        printf("Usage: %s <num_threads> <start> <end>\n", argv[0]);
        return -1;
    }
    if(sscanf(argv[1], "%u", &num_threads) != 1){
        printf("invalid number of threads\n");
        return -1;
    }
    if(sscanf(argv[2], "%u", &start_value) != 1){
        printf("invalid start\n");
        return -1;
    }
    if(sscanf(argv[3], "%u", &end_value) != 1){
        printf("invalid end\n");
        return -1;
    }

    workers = calloc(num_threads, sizeof(*workers));
    for(i=0; i<num_threads; i+=1) {
        workers[i].myid = i;
        workers[i].state = WS_IDLE;
        pthread_create(&workers[i].tid, NULL, worker, workers + i);
    }

    while (start_value <= end_value) {
        for(i=0; i <num_threads; i+=1) {
            if(workers[i].state == WS_IDLE) {
                workers[i].value = start_value++;
                workers[i].state = WS_WORK;
            }
        }
    }

    for(i=0; i < num_threads; i+=1){
        while (workers[i].state !=  WS_IDLE);
        workers[i].state = WS_DIE;
        pthread_join(workers[i].tid, NULL);
    }

    free(workers);

}

static void *worker(void *arg) {
    Worker *w = (Worker *)arg;
    unsigned int i;
    for (;;) {
        while (w->state == WS_IDLE){
            usleep(1000);
        }
        if(w->state == WS_DIE){
            break;
        }

        w->is_prime = 1;
        for(i = 2; i < w->value; i+=1) {
            if(w->value%i == 0){
                w->is_prime = 0;
            }
        }
        printf("Thread %u: %u is %s\n", w->myid, w->value, w->is_prime ?
        "prime" : "not prime"); w->state = WS_IDLE;
    }
    return NULL;
}

// #include <pthread.h>
// #include <stdbool.h>
// #include <stddef.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #define HAVE_POLL_DELAY
// #ifdef HAVE_POLL_DELAY
// const struct timespec POLL_DELAY = {0, 10000};
// #endif

// typedef enum { WS_IDLE = 0, WS_WORK, WS_DIE } WorkState;

// typedef struct {
//   pthread_t tid;
//   unsigned int value;
//   bool result;
//   WorkState state;
// } Worker;

// static void *worker(void *arg);

// int main(int argc, char *argv[]) {
//   Worker *work;
//   unsigned int num_workers;
//   unsigned int start_value;
//   unsigned int end_value;
//   unsigned int value_assigned;
//   unsigned int i;
//   unsigned int j;
//   if (argc < 4) {
//     printf("Usage: %s <num workers> <start value> <end value>\n", argv[0]);
//     return -1;
//   }
//   if (1 != sscanf(argv[1], "%u", &num_workers) || num_workers == 0) {
//     printf("Invalid number of workers '%s'\n", argv[1]);
//     return -1;
//   }
//   if (1 != sscanf(argv[2], "%u", &start_value)) {
//     printf("Invalid start value '%s'\n", argv[2]);
//     return -1;
//   }
//   if (1 != sscanf(argv[3], "%u", &end_value)) {
//     printf("Invalid end value '%s'\n", argv[3]);
//     return -1;
//   }
//   if (start_value > end_value) {
//     printf("Invalid start value: is > end value.\n");
//     return -1;
//   }
//   work = calloc(num_workers, sizeof(*work));
//   for (i = 0; i < num_workers; i += 1) {
//     work[i].state = WS_IDLE;
//     pthread_create(&work[i].tid, NULL, worker, work + i);
//   }
//   value_assigned = start_value;
//   while (value_assigned <= end_value) {
//     for (i = 0; i < num_workers && value_assigned <= end_value; i += 1) {
//       work[i].value = value_assigned++;
//       work[i].state = WS_WORK;
//     }
//     for (j = 0; j < i; j += 1) {
//       while (work[j].state != WS_IDLE) {
// #ifdef HAVE_POLL_DELAY
//         nanosleep(&POLL_DELAY, NULL);
// #endif
//       }
//       printf("thread %u: %u %s\n", work[j].myid, work[j].value, work[j].result ? "prime" : "not prime");
//     }
//   }
//   for (i = 0; i < num_workers; i += 1) {
//     work[i].state = WS_DIE;
//     pthread_join(work[i].tid, NULL);
//   }
//   free(work);
// }

// static void *worker(void *arg) {
//   Worker *w = (Worker *)arg;
//   long i;
//   for (;;) {
//     // Hang here until the marshaller thread gives us work to do.
//     while (w->state == WS_IDLE) {
// #ifdef HAVE_POLL_DELAY
//       nanosleep(&POLL_DELAY, NULL);
// #endif
//     }
//     // It is still possible we go from WS_IDLE -> WS_DIE. If we didn't
//     // have this check, we would have to do work before we get out.
//     if (w->state == WS_DIE) {
//       break;
//     }
//     w->result = true;
//     for (i = 2; i < w->value; i += 1) {
//       if ((w->value % i) == 0) {
//         w->result = false;
//         // Remove the break to make the calculation slower so
//         // we can see the threading benefits.
//         // break;
//       }
//     }
//     w->state = WS_IDLE;
//   }
//   return NULL;
// }