#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    pid_t pid;
    int ret = 1;
    int status;
    // pid = fork();
    fork();
    fork();
    fork();

    // if(pid == 0){
    //     printf("Child PID %u\n", getpid());
    // }
    // else if (pid > 0){
    //     printf("Parent of child %u\n", pid);
    //     waitpid(pid, NULL, 0);
    // }
    // else {
    //     perror("fork");
    // }
    // if(pid < 0){
    //     perror("fork failed");
    //     exit(1);
    // }
    printf("forked\n");
}