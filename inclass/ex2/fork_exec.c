#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char *argv[])
{
    pid_t pid;
    if (argc < 2) {
        printf("Usage: %s <command> [arguments]\n", argv[0]);
        return -1;
    }
    // Exec is going to take over the calling process, so we fork so that we
    // are still in charge of it when the command executes.
    pid = fork();
    if (pid == 0) { // CHILD
        int i;
        char **args;
        // The list must be terminated by NULL, so we need to allocate one
        // more slot than we actually need. 
        args = calloc(argc, sizeof(*args));
        for (i = 1;i < argc;i+=1) {
            args[i - 1] = argv[i];
        }
        execvp(args[0], args);
        // If execvp succeeds, it doesn't return, so if we get here, there was
        // an error.
        perror(args[0]);
        free(args);
        return -1;
    }
    else if (pid > 0) { // PARENT
        int status;
        waitpid(pid, &status, 0);
        printf("\n");
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == 0) {
                printf("Execution successful!\n");
            }
            else {
                printf("Execution failed with code %u.\n", WEXITSTATUS(status));
            }
        }
        else if (WIFSIGNALED(status)) {
            printf("Execution terminated on signal %u.\n", WTERMSIG(status));
        }
    }
    else {
        perror("fork");
        return -1;
    }
return 0;
}
