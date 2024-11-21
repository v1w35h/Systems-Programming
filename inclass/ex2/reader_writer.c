#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

#define DEFAULT_RDTHREADS       3
#define DEFAULT_WRTHREADS       3
#define DEFAULT_NUM_TRIALS      200000

// Data to be shared amongst all children and the parent.
typedef struct {
    sem_t rdlock;   // Reader's lock
    sem_t wrlock;   // Writer's lock
    sem_t flock;    // fault lock, access to sync 
    int rdcount;    // Number of readers
    int wrcount;    // Number of times a writer wrote.
    int value;      // Value we want to read/write
    int faults;
} Data;

unsigned int TRIALS = DEFAULT_NUM_TRIALS;
unsigned int RDTHREADS = DEFAULT_RDTHREADS;
unsigned int WRTHREADS = DEFAULT_WRTHREADS;
int (*wrt)(Data *) = NULL;

// "good" thread, with semaphore.
int gwr_thread(Data *d)
{
    unsigned int i;

    for (i = 0;i < TRIALS;i+=1) {
        // good way 1 writer
        sem_wait(&d->wrlock);
        // We can't enter this critical section without acquiring
        // the write lock.
        d->value += 1;
        d->wrcount += 1;
        sem_post(&d->wrlock);
    }

    return 0;
}

// "bad" thread, without semaphore.
int bwr_thread(Data *d)
{
    unsigned int i;

    for (i = 0;i < TRIALS;i+=1) {
        // There is no exclusive access to the following
        // which can lead to race conditions.
        d->value += 1;
        d->wrcount += 1;
    }

    return 0;
}

int rd_thread(Data *d)
{
    unsigned int i;
    
    for (i = 0;i < TRIALS;i+=1) {
        // // span of read count
        sem_wait(&d->rdlock);
        // We now have exclusive access to rdcount
        d->rdcount += 1;
        if (d->rdcount == 1) {
            // We are the first reader, exclude writers.
            sem_wait(&d->wrlock);
        }
        // We no longer need rdcount
        sem_post(&d->rdlock);
        // // up to here

        if (d->value != d->wrcount) {
            sem_wait(&d->flock);
            d->faults += 1;
            sem_post(&d->flock);
        }

        // Now that we're done reading, we need access to
        // rdlock.
        sem_wait(&d->rdlock);
        d->rdcount -= 1;
        if (d->rdcount == 0) {
            // We were the last reader, allow writers.
            sem_post(&d->wrlock);
        }
        sem_post(&d->rdlock);
    }

    return 0;
}

// Globals so that when we exit for some reason, it gets freed
// appropriately.
Data *data;
pid_t *wrthreads;
pid_t *rdthreads;

void when_i_exit(void) {
    if (data != MAP_FAILED) {
        munmap(data, sizeof(*data));
    }
    shm_unlink("/shmrw");
    free(wrthreads);
    free(rdthreads);
}

void when_i_int(int sig) {
    if (sig == SIGINT) {
        exit(EXIT_SUCCESS);
    }
}


int main(int argc, char *argv[])
{
    unsigned int i;
    int shmfd;

    for (int i = 1;i < argc;i+=1) {
        if (!strcmp(argv[i], "-g")) {
            wrt = gwr_thread;
        }
        else if (!strcmp(argv[i], "-b")) {
            wrt = bwr_thread;
        }
        else if (!strcmp(argv[i], "-t")) {
            if (argc <= i + 1) {
                printf("Trials requires a number.\n");
                return -1;
            }
            i += 1;
            if (1 != sscanf(argv[i], "%u", &TRIALS)) {
                printf("Unable to scan trials '%s'\n", argv[i]);
                return -1;
            }
        }
        else if (!strcmp(argv[i], "-r")) {
            if (argc <= i + 1) {
                printf("Read threads requires a number.\n");
                return -1;
            }
            i += 1;
            if (1 != sscanf(argv[i], "%u", &RDTHREADS) || RDTHREADS < 3 || RDTHREADS > 200) {
                printf("Unable to scan read threads '%s'.\n", argv[i]);
                return -1;
            }                
        }
        else if (!strcmp(argv[i], "-w")) {
            if (argc <= i + 1) {
                printf("Write threads requires a number.\n");
                return -1;
            }
            i += 1;
            if (1 != sscanf(argv[i], "%u", &WRTHREADS) || WRTHREADS < 3 || WRTHREADS > 200) {
                printf("Unable to scan write threads '%s'.\n", argv[i]);
                return -1;
            }                
        }
        else {
            printf("Unknown argument '%s'\n", argv[i]);
            return -1;
        }
    }

    if (wrt == NULL) {
        printf("Usage: %s <-b|-g> [-t num trials] [-w num write threads] [-r num read threads]\n", argv[0]);
        return -1;
    }

    // Create the shared memory before fork'ing so that it can be used with the other
    // forked processes.
    shmfd = shm_open("/shmrw", O_CREAT | O_EXCL | O_RDWR, 0600);
    if (shmfd < 0) {
        perror("shm_open");
        return -1;
    }
    // Resize the shared memory.
    ftruncate(shmfd, sizeof(*data));
    // Now we map it with MAP_SHARED so all processes can see writes.
    data = mmap(NULL, sizeof(*data), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    // mmap is now holding the reference count, or if mmap fails, this
    // needs to be closed anyway.
    close(shmfd);
    if (data == MAP_FAILED) {
        perror("mmap");
        shm_unlink("/shmrw");
        return -1;
    }

    // Before we allocate the threads, we can initialize the
    // semaphores. If we don't specify pshared = 1, then the
    // semaphore will expect that all threads share the same
    // memory, but in this case, they do not. Instead, we are
    // using the SHM construct to share the semaphore.
    sem_init(&data->rdlock, 1, 1);
    sem_init(&data->wrlock, 1, 1);
    sem_init(&data->flock, 1, 1);
    data->value = 0;
    data->faults = 0;

    // Keep track of the PIDs so we can see them later. We
    // can use wait(NULL), but it's best to wait on each
    // pid so that we make sure we join every child.
    wrthreads = calloc(WRTHREADS, sizeof(pid_t));
    rdthreads = calloc(RDTHREADS, sizeof(pid_t));

    for (i = 0;i < WRTHREADS;i+=1) {
        wrthreads[i] = fork();
        if (wrthreads[i] == 0) {
            return wrt(data);
        }
    }

    for (i = 0;i < RDTHREADS;i+=1) {
        rdthreads[i] = fork();
        if (rdthreads[i] == 0) {
            return rd_thread(data);
        }
    }

    atexit(when_i_exit);
    signal(SIGINT, when_i_int);

    for (i = 0;i < WRTHREADS;i+=1) {
        waitpid(wrthreads[i], NULL, 0);
    }
    for (i = 0;i < RDTHREADS;i+=1) {
        waitpid(rdthreads[i], NULL, 0);
    }

    sem_destroy(&data->rdlock);
    sem_destroy(&data->wrlock);
    sem_destroy(&data->flock);

    if (data->value != data->wrcount) {
        printf("ERROR: Values are not correct. (%d faults)\n", data->faults);
    }
    else {
        printf("CONGRATS: Values are correct.\n");
    }

    return 0;
}
