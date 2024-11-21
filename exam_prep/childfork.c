#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(void){
    printf ("main process: %d\n", (int)getpid());

pid_t pid = fork();


if(pid == 0) {
    printf ("Child process: %d\n", (int)getpid());
    printf ("Child's parent process: %d\n", (int)getppid());
    pid_t pid2= fork();

    if ((pid2) == 0) {
        printf("Child's child process: %d\n", (int)getpid());
    } else {
        int st2;
        wait(&st2);
    }
} else {
    printf ("Parent process: %d\n", (int)getpid());
    int st;
    wait(&st);
    printf("child's pid: %ld\n", pid);
}

return 0;
}