#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <time.h>

static void test(void){
    // struct timespec ts = {10,0};
    // nanosleep(&ts,NULL);
    unsigned long i;
    for(i = 0; i < 1000000UL; i++);
}

int main(){
    struct rlimit r1;
    pid_t pid;
    getrlimit(RLIMIT_CPU, &r1);

    printf("%zu %zu\n", r1.rlim_cur, r1.rlim_max);

    pid = fork();
    if(pid == 0){
        r1.rlim_max = 2;
        r1.rlim_cur = 2;
        setrlimit(RLIMIT_CPU, &r1);
        getrlimit(RLIMIT_CPU, &r1);
        printf("%zu %zu\n", r1.rlim_cur, r1.rlim_max);
        test();
        printf("Finish test\n");
    }
    else if(pid > 0){
        int stat;
        while(1){
            int val = waitpid(pid, &stat, WNOHANG); // zero specifies no flags
            printf("val = %d sig? %d exit? %d\n", val, WIFSIGNALED(stat), WEXITSTATUS(stat));
            if(val > 0) break;
        }
    }
}