// Demonstrates using message queues to 
// command a thread to calculate a range of
// primes.
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mqueue.h>

#define NUM_THREADS  5
#define PRIME_SPAN   10

//running one thread at a time, if we did one number at a time this would be expensive
typedef struct {
    // Find primes between [start..end]
    unsigned int start;
    unsigned int end;
} Command;

// doesn't rely on any  significant order
bool test_prime(unsigned int value)
{
    unsigned int i;
    for (i = 2;i * i <= value;i+=1) {
        if ((value % i) == 0) {
            return false;
        }
    }
    return true;
}

int thread_worker(mqd_t mqueue)
{
    char *message;
    struct mq_attr attr;
    unsigned int priority = 0;
    Command *cmd;
    unsigned int i;

    if (mq_getattr(mqueue, &attr) < 0) {
        return 1;
    }

    message = (char *)calloc(attr.mq_msgsize, sizeof(char));
    cmd = (Command *)message;

    // The message is bigger than a command, but we only care about the command.
    if ((mq_receive(mqueue, message, attr.mq_msgsize, &priority)) < 0) {
        perror("mq_receive");
        return 1;
    }

    mq_close(mqueue);
    for (i = cmd->start;i <= cmd->end;i += 1) {
        if (test_prime(i)) {
            printf("(child %6d): %6d is prime!\n", getpid(), i);
        }
    }
    free(message);
    return 0;
}

int main(void)
{
    int i;
    mqd_t mqueue;
    Command cmd;
    pid_t *pids = calloc(NUM_THREADS, sizeof(pid_t));

    // open the message queue
    mqueue = mq_open("/prime.mq", O_CREAT | O_EXCL | O_RDWR, 0600, NULL); // NULL decides defualt attribute
    if (mqueue < 0) {
        perror("parent mq_open");
        return -1;
    }

    for (i = 0;i < NUM_THREADS;i++) {
        if ((pids[i] = fork()) == 0) {
            exit(thread_worker(mqueue)); // testing the primes
        }
    }

    // We do this so we can demonstrate message queues
    for (i = 0;i < NUM_THREADS;i++) {
        cmd.start = i * PRIME_SPAN + 2;
        cmd.end = cmd.start + (PRIME_SPAN - 1);
        mq_send(mqueue, (char*)&cmd, sizeof(cmd), 1); // all priority 1, has to be char* bc 1 byte
    }

    for (i = 0;i < NUM_THREADS;i++) {
        waitpid(pids[i], NULL, 0); // joining, clean up the children
    }
    mq_close(mqueue); // cleanup
    mq_unlink("/prime.mq");
    free(pids);

    return 0;
}
