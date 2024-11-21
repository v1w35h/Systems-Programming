/* Viwesh Gupta
thread pool lab: making a pool of threads to run hashing functions in parallel to print hashes for all the files in a specified directory
got a lot of help from Gabriel
also Jacob helped me get started
I also got good help from John Vargas who told me that I could use a lot of the code from work_ordered.c
*/

#include <dirent.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
  unsigned long return_val; // return value of the hash
  bool done;                // job is done
  unsigned int hash;        // type of hash
  char *name;               // the file name
} job;

// from work_ordered.c
typedef struct {
  job **buffer;
  unsigned int size;
  unsigned int at;
  unsigned int capacity;
} RingBuffer;

typedef struct {
  // from in class example/ work_ordered.c
  RingBuffer *rb;
  bool die;
  pthread_cond_t not_empty;
  pthread_cond_t not_full;
  pthread_cond_t job_done;
  pthread_mutex_t mutex;
} WorkerQueue;

typedef struct {
  // struct for the thread pool
  pthread_t *tids;
  unsigned int n_threads;
  unsigned int vec_size;
  WorkerQueue *queue;
  job *j;
} tpool;

// ringbuffer functions from work_ordered.c
static RingBuffer *ring_new(unsigned int capacity);
static void ring_free(RingBuffer *rb);
static WorkerQueue *queue_new(unsigned int capacity);
static void queue_free(WorkerQueue *q);
static void queue_push(WorkerQueue *q, job *j);
static job *queue_pop(WorkerQueue *q);
// worker function to use in pthread_create
static void *worker(void *arg);

void *thread_pool_init(int num_threads) {
  // Initialize num_threads workers and return handle
  if (num_threads < 1 || num_threads > 32) {
    return NULL;
  }
  tpool *t;
  t = malloc(sizeof(tpool));
  if (!t) {
    return NULL;
  }
  if (!t->queue) {
    free(t);
    return NULL;
  }

  unsigned int i;
  // initialize cond vars and mutex
  t->queue = queue_new(num_threads);
  // allocate memory for threads & jobs
  t->tids = calloc(num_threads, sizeof(pthread_t));
  t->n_threads = num_threads;
  t->vec_size = num_threads;
  t->j = calloc(t->vec_size, sizeof(job));
  // set die to false 
  t->queue->die = false;

  // start threads
  for (i = 0; i < (unsigned int)num_threads; i += 1) {
    if (pthread_create(&t->tids[i], NULL, worker, t->queue) != 0) {
      return NULL;
    }
  }

  return t;
}

bool thread_pool_hash(void *handle, const char *directory, int hash_size) {
  // Assign work to the pool of threads
  // error check passed in args
  if (handle == NULL || directory == NULL) {
    return false;
  }

  // set the handle to be the thread pool initialized in init
  tpool *t = (tpool *)handle;

  int h_size;

  // hash sizes other than 32 or 64 will not work
  if (hash_size != 32 && hash_size != 64) {
    return false;
  }
  // set the printf specifier for each hash here
  if (hash_size == 32) {
    h_size = 8;
  }
  if (hash_size == 64) {
    h_size = 16;
  }

  // need DIR & dirent for opening and reading directories
  DIR *dir;
  struct dirent *DENT;
  struct stat st;
  // Originally I had char*, but Gabriel helped me with this part to avoid the
  // seg fault I was getting
  char fpath[1024];

  // open the directory & error check
  dir = opendir(directory);
  if (dir == NULL) {
    return false;
  }

  unsigned int i;

  // from the lesson on posix directories
  while ((DENT = readdir(dir)) != NULL) {
    // got this from Gabriel's office hours
    sprintf(fpath,"%s/%s", directory, DENT->d_name);
    stat(fpath, &st);
    // basically check if the file path is regular (not '.' or '..')
    if (S_ISREG(st.st_mode)) {
      // push jobs to the queue
      for (i = 0; i < t->queue->rb->capacity; i += 1) {
        t->j[i].name = fpath;
        t->j[i].hash = hash_size;
        t->j[i].done = false;
        queue_push(t->queue, t->j+i);

      }
      // lock the mutex
      pthread_mutex_lock(&t->queue->mutex);

      // while the job is not done, block on the job_done conditional
      while(!t->j->done){
        pthread_cond_wait(&t->queue->job_done, &t->queue->mutex);
      }
      // unlock the mutex and then print the hash
      pthread_mutex_unlock(&t->queue->mutex);
      printf("%0*lx: %s\n", h_size, t->j->return_val, t->j->name);
    }

  }

  // close the directory
  closedir(dir);

  return true;
}

void thread_pool_shutdown(void *handle) {
  // Clean up and stop the threads
  // error check passed in arg
  if (handle == NULL) {
    return;
  }

  // set the handle to be the thread pool initialized in init
  tpool *t = (tpool *)handle;
  unsigned int i;

  t->queue->die = true;

  pthread_mutex_lock(&t->queue->mutex);
  // I didn't quite understand what this means, but I saw that Dr. Marz did it 
  pthread_cond_broadcast(&t->queue->not_empty);
  pthread_mutex_unlock(&t->queue->mutex);

  // join the threads 
  for (i = 0; i < t->n_threads; i += 1) {
    pthread_join(t->tids[i], NULL);
  }
  
  // free all the things that allocated memory
  queue_free(t->queue);
  free(t->tids);
  free(t->j);
  free(t);
  return;
}

uint32_t hash32(FILE *FL) {
  int byte;
  uint32_t digest = 2166136261U;
  // grab bytes until end of file
  while ((byte = getc(FL)) != EOF) {
    digest ^= byte; // xor byte
    digest *= 16777619U;
  }
  return digest;
}

uint64_t hash64(FILE *FL) {
  int byte;
  // had to cast to UL to avoid warnings
  uint64_t digest = 14695981039346656037UL;
  while ((byte = getc(FL)) != EOF) {
    digest ^= byte; // xor byte
    digest *= 1099511628211UL;
  }
  return digest;
}

// I copied these directly from Dr. Marz's code 
// (changing names to match what I named my variables)
static RingBuffer *ring_new(unsigned int capacity) {
  RingBuffer *rb;
  rb = malloc(sizeof(*rb));
  rb->at = 0;
  rb->capacity = capacity;
  rb->size = 0;
  rb->buffer = calloc(capacity, sizeof(job *));
  return rb;
}
static void ring_free(RingBuffer *rb) {
  free(rb->buffer);
  free(rb);
}
static WorkerQueue *queue_new(unsigned int capacity) {
  WorkerQueue *queue;
  queue = malloc(sizeof(*queue));
  queue->rb = ring_new(capacity);
  queue->die = false;
  pthread_mutex_init(&queue->mutex, NULL);
  pthread_cond_init(&queue->not_empty, NULL);
  pthread_cond_init(&queue->not_full, NULL);
  pthread_cond_init(&queue->job_done, NULL);
  return queue;
}

static void queue_free(WorkerQueue *queue) {
  ring_free(queue->rb);
  pthread_mutex_destroy(&queue->mutex);
  pthread_cond_destroy(&queue->not_empty);
  pthread_cond_destroy(&queue->not_full);
  pthread_cond_destroy(&queue->job_done);
  free(queue);
}

static void queue_push(WorkerQueue *queue, job *task) {
  pthread_mutex_lock(&queue->mutex);
  // Wait if queue is full
  while (queue->rb->size >= queue->rb->capacity && !queue->die) {
    pthread_cond_wait(&queue->not_full, &queue->mutex);
  }
  if (queue->die) {
    // Don't add work if the die signal has been sent. This should
    // not happen since queue_push is only called by the marshaller,
    // but it is a check against having a worker perhaps push it.
    pthread_mutex_unlock(&queue->mutex);
    return;
  }
  queue->rb->buffer[(queue->rb->at + queue->rb->size) % queue->rb->capacity] = task;
  queue->rb->size += 1;
  // Signal a waiting worker that the queue is no longer empty
  pthread_cond_signal(&queue->not_empty);
  pthread_mutex_unlock(&queue->mutex);
}

static job *queue_pop(WorkerQueue *queue) {
  job *j;
  pthread_mutex_lock(&queue->mutex);
  // Wait if queue is empty
  while (queue->rb->size == 0 && !queue->die) {
    // The mutex will be unlocked by pthread_cond_wait
    pthread_cond_wait(&queue->not_empty, &queue->mutex);
  }
  if (queue->die) {
    // Remember that we hold the mutex lock after pthread_cond_wait returns
    pthread_mutex_unlock(&queue->mutex);
    return NULL;
  }
  j = queue->rb->buffer[queue->rb->at];
  queue->rb->at = (queue->rb->at + 1) % queue->rb->capacity;
  queue->rb->size -= 1;
  // Signal a thread waiting to add a task that the queue is no longer full
  pthread_cond_signal(&queue->not_full);
  pthread_mutex_unlock(&queue->mutex);
  return j;
}

static void *worker(void *arg) {
  // set the queue to be the argument, it was already given in pthread_create
  WorkerQueue *q = (WorkerQueue *)arg;
  job *j;


  // while jobs can be popped off the q
  while((j = queue_pop(q)) != NULL){
    // use fopen to read the file
    FILE *FL = fopen(j->name, "r");

    if (FL) {
      // based on the type of hash you're doing store the return value
      // of that hash function
      if (j->hash == 32) {
        j->return_val = hash32(FL);
      } else if (j->hash == 64) {
        j->return_val = hash64(FL);
      }

    }
    // set the job to done
    j->done = true;
    // lock the mutex
    pthread_mutex_lock(&q->mutex);
    // signal that the job is done
    pthread_cond_signal(&q->job_done);
    // unlock the mutex
    pthread_mutex_unlock(&q->mutex);
    // close the file
    fclose(FL);
  }

  return NULL;
}
