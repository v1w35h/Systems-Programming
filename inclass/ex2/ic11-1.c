#include <time.h>
#include <sys/times.h>
#include <stdio.h>
#include <string.h>

int main(){
    struct tm mytime;
    time_t t;
    char buf[26];

    t = time(NULL);
    // printf("%lu\n", t);

    localtime_r(&t, &mytime);
    // printf("%d-%d-%d\n", mytime.tm_mon, mytime.tm_mday, mytime.tm_year);
    // mytime.tm_year = 1970 -1900;
    // mytime.tm_mon = 0;
    // mytime.tm_mday = 1;
    // printf("%lu\n", mktime(&mytime));

    // same thing printed, different 
    asctime_r(&mytime, buf);

    printf("%s", buf);

    ctime_r(&t, buf);

    printf("%s", buf);


    // void *ptr = memset(malloc(1234), 0 ,1234);
    // calloc 

}