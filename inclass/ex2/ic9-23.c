#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main(void)  {
    pid_t pid;

    printf("Hello World\n");
    pid = fork();
    // fork();
    // parent gets the pid child
    // child gets 0

    if(pid == 0){
        sleep(10);
        printf("Child\n");
    }
    else if(pid > 0) {
        // sleep(1);

        printf("Parent! Child pid is %d\n",pid);
        waitpid(pid,NULL,0);

        // exit(EXIT_SUCCESS);
    }
    else {
        perror("Fork");
    }
}