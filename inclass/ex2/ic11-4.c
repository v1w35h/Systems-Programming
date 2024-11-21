#include <time.h>

#include <sys/times.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// clock_t is a long 

// struct tms {
//     clock_t tms_utime; // user time
//     clock_t tms_stime; //sys time
//     clock_t tms_cutime; // user time of children
//     clock_t tms_cstime; //system  time of children
// };

// struct itimerval {
//     struct timeval 
// }

int main(){
    struct tm mytime;
    time_t t;
    char buf[32];

    t = time(NULL);
    // printf("%lu\n", t);

    localtime_r(&t, &mytime);
    strftime(buf, 32, "%H:%M:%S  %d/%m/%Y", &mytime);
 
    puts(buf);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // printf("%lu\n", clock());
    // sleep(1);
    // printf("%lu\n", clock());
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // clock_t st, ed;

    // st =clock();
    // for(volatile int i = 0; i <10000; i+=1);
    // ed = clock();

    // printf("Time = %lu\n", ed-st);
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // unsigned int j;
    // j = sleep(12);
    // for(int i =0; i < j; i++){
    //     printf("%d\n", i);
    // }
    // printf("%u", j);

}