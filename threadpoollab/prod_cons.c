#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct {
    unsigned int value;
    bool is_prime;
    bool done;
} Task;

typedef struct{
    Task    **buffer;
    unsigned int size;
    unsigned int at;
    unsigned int capacity;
    bool        die;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
    pthread_cond_t job_done;
    pthread_mutex_t mutex;
} Queue;

static void *worker(void *arg);

int main(int argc, char *argv[]){
    Queue queue;
    pthread_t *tids;
    Task *tasks;
    unsigned int queue_size;
    // bigger queue size means issue more jobs
    unsigned int vector_size;
    // if q size is less than vec size, we have to wait this sucks
    // if q size is bigger than vec size, 
    // q size = vec size is best solution
    unsigned int num_threads;
    unsigned int start_value;
    unsigned int end_value;
    unsigned int i;
    unsigned int z;
    if(argc != 6){
        printf("Usage: %s <queue_size> <vector_size> <num_threads> <start> <end>\n", argv[0]);
        return -1;
    }
    if(sscanf(argv[1], "%u", &queue_size) != 1){
        printf("invalid queue size\n");
        return -1;
    }
    if(sscanf(argv[2], "%u", &vector_size) != 1){
        printf("invalid vector size\n");
        return -1;
    }
    if(sscanf(argv[3], "%u", &num_threads) != 1){
        printf("invalid number of threads\n");
        return -1;
    }
    if(sscanf(argv[4], "%u", &start_value) != 1){
        printf("invalid start\n");
        return -1;
    }
    if(sscanf(argv[5], "%u", &end_value) != 1){
        printf("invalid end\n");
        return -1;
    }

    queue.capacity = queue_size;
    queue.size = 0;
    queue.at = 0;
    queue.buffer = calloc(queue_size, sizeof(Task*));
    queue.die = false;
    pthread_cond_init(&queue.not_empty, NULL);
    pthread_cond_init(&queue.not_full, NULL);
    pthread_cond_init(&queue.job_done, NULL);
    pthread_mutex_init(&queue.mutex,  NULL);

    tids = calloc(num_threads, sizeof(pthread_t));
    tasks = calloc(vector_size, sizeof(Task));

    for(i = 0; i < num_threads; i++){
        pthread_create(tids+i, NULL, worker, &queue);
    }

    while(start_value <= end_value){
        // task = malloc(sizeof(*task)); no longer needed
        for(i = 0; i < vector_size && start_value <= end_value; i+=1){
            tasks[i].value = start_value++;
            tasks[i].done = false;

            pthread_mutex_lock(&queue.mutex);
            while(queue.size == queue.capacity) {
                pthread_cond_wait(&queue.not_full, &queue.mutex);
            }

            queue.buffer[(queue.at + queue.size) %queue.capacity] = tasks + i;
            queue.size += 1;
            pthread_cond_signal(&queue.not_empty);
            pthread_mutex_unlock(&queue.mutex);
        }
        // after this for loop we wait for work to be done

        for(z=0; z< i; z+=1){
            pthread_mutex_lock(&queue.mutex);
            while(!tasks[z].done){
                // usleep(100);
                // unlocks the mutex and then sleeps on the cond var
                pthread_cond_wait(&queue.job_done, &queue.mutex);
            }
            // mutex is currently locked, doesn't matter whether we unlock before or after printf
            // done is true, can now print out the work
            pthread_mutex_unlock(&queue.mutex);
            printf("%u: is%s prime.\n", tasks[z].value, tasks[z].is_prime ? "" : " not");
        }
    }

    while(queue_size > 0){
        usleep(100);
    }

    queue.die = true;

    pthread_mutex_lock(&queue.mutex);
    pthread_cond_broadcast(&queue.not_empty);
    pthread_mutex_unlock(&queue.mutex);
    
    for(i = 0; i < num_threads; i++){
        pthread_join(tids[i], NULL);
    }

    //clean up

    pthread_cond_destroy(&queue.not_empty);
    pthread_cond_destroy(&queue.not_full);
    pthread_cond_destroy(&queue.job_done);
    pthread_mutex_destroy(&queue.mutex);
    free(tids);
    free(tasks);
    free(queue.buffer);

    return 0;

}

static void *worker(void *arg){
    Queue *q = (Queue *)arg;
    Task *task;
    // bool is_prime; move to task struct
    unsigned int i;

    for(;;){
        pthread_mutex_lock(&q->mutex);
        while(q->size == 0 && !q->die){
            pthread_cond_wait(&q->not_empty, &q->mutex);
        }
        if(q->die){
            pthread_mutex_unlock(&q->mutex);
            return NULL;
        }
        task = q->buffer[q->at];
        q->at  = (q->at +1) % q->capacity;
        q->size -= 1;
        pthread_cond_signal(&q->not_full);
        pthread_mutex_unlock(&q->mutex);
        task->is_prime = true;
        if (task->value < 2) {
            task->is_prime = false;
        } else {
            for (i = 2; i * i <= task->value; i++) {
                if ((task->value % i) == 0) {
                    task->is_prime = false;
                    break;
                }
            }
        }

        task->done = true;
        pthread_mutex_lock(&q->mutex);
        pthread_cond_signal(&q->job_done);
        // when we acquire a signal the mutex locks
        pthread_mutex_unlock(&q->mutex);
    }
    // printf("%u is%s prime.\n", task->value, is_prime ? "" : " not");
    return NULL;
}