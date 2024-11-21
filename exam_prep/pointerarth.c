#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/resource.h>

// typedef struct {
//     int x;
//     double y;
// } StructExample;

// int main() {
//     // StructExample *se = (StructExample *)3200;
//     // printf("size of an int: %lu\n answer is %lu ->x\n",sizeof(int), &(se + 3)->x);
//     // printf("size of a double: %lu\n answer is %lu ->y\n",sizeof(double), &(se + 3)->y);
//     int ***a = (int ***)3578;
//     printf("answer is %lu ->x\n", a+17);

// }

// #include <unistd.h>
// #include <stdio.h>
// #include <sys/wait.h>
// #include <sys/resource.h>
// #include <time.h>

static void token(void){
    // struct timespec ts = {10,0};
    // nanosleep(&ts, NULL);

    // for(int i= 0; i < 10000000000UL; i++);
    for(;;);

    
}

int main() {
//     // pid_t pid;
//     // printf("Parent only.\n");
    
//     // pid = fork();
    
//     // if (pid < 0) {
//     //     perror("fork");
//     // } 
//     // else if (pid == 0) {
//     //     printf("CHILD!\n");
//     // } 
//     // else {
//     //     printf("Parent! Child is PID %d\n", pid);
//     //     int wstatus;
//     //     wait(&wstatus);

//     //     if (WIFEXITED(wstatus)) {
//     //         printf("Child exited with status %d\n", WEXITSTATUS(wstatus));
//     //     }
//     // }
    
//     // return 0;

//     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    struct rlimit rt;

    // getrlimit(RLIMIT_CPU, &rt);
    // printf("%lu  %lu\n", rt.rlim_cur, rt.rlim_max);

    rt.rlim_cur = 1;
    rt.rlim_max = 100;
    setrlimit(RLIMIT_CPU, &rt);
    getrlimit(RLIMIT_CPU, &rt);

    printf("%lu  %lu\n", rt.rlim_cur, rt.rlim_max);

    token();
    printf("DONE\n");

//     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // printf("My pid: %d Parent PID: %d\n", getpid(), getppid());
}

