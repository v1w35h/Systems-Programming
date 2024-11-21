#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fds[2];
    int i =  33;
    pid_t pid;

    pipe(fds); // 0 -read, 1 -write
    pid = fork();
    if(pid == 0){
        close(fds[0]);
        i = -111;
        write(fds[1],&i,sizeof(i));
        printf("CHILD: %d %p\n", i, &i);
    }
    if(pid > 0){
        int newi;
        close(fds[1]);
        read(fds[0],&newi,sizeof(newi));
        waitpid(pid,NULL,0);
        printf("PARENT: %d %p, %d %p\n", i, &i, newi, &newi);
    }
}