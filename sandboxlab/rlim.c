#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

void resourceLims(char **argv) {
    struct rlimit rlim;
    int argc = 0;

    // Calculate number of arguments
    while (argv[argc] != NULL) {
        argc++; // count the number of arguments
    }

    // Set default limits for the resources
    setrlimit(RLIMIT_NPROC, &(struct rlimit){256, 256});      // max processes -p
    setrlimit(RLIMIT_DATA, &(struct rlimit){1 << 30, 1 << 30}); // max data -d
    setrlimit(RLIMIT_STACK, &(struct rlimit){1 << 30, 1 << 30}); // max stack -s
    setrlimit(RLIMIT_NOFILE, &(struct rlimit){256, 256});     // max open files -n
    setrlimit(RLIMIT_FSIZE, &(struct rlimit){1 << 30, 1 << 30}); // max file size -f
    setrlimit(RLIMIT_CPU, &(struct rlimit){1 << 30, 1 << 30});  // max CPU time -t

    int opt;
    int rlim_val;

    // Parse command-line options
    while ((opt = getopt(argc, argv, "p:d:s:n:f:t")) != -1) {
        if (optarg != NULL && sscanf(optarg, "%d", &rlim_val) == 1) {
            switch (opt) {
                case 'p':
                    // Maximum number of processes
                    rlim.rlim_cur = rlim_val;
                    rlim.rlim_max = rlim_val;
                    setrlimit(RLIMIT_NPROC, &rlim);
                    break;
                case 'd':
                    // Maximum data area size
                    rlim.rlim_cur = rlim_val;
                    rlim.rlim_max = rlim_val;
                    setrlimit(RLIMIT_DATA, &rlim);
                    break;
                case 's':
                    // Maximum stack size
                    rlim.rlim_cur = rlim_val;
                    rlim.rlim_max = rlim_val;
                    setrlimit(RLIMIT_STACK, &rlim);
                    break;
                case 'n':
                    // Maximum number of open file descriptors
                    rlim.rlim_cur = rlim_val;
                    rlim.rlim_max = rlim_val;
                    setrlimit(RLIMIT_NOFILE, &rlim);
                    break;
                case 'f':
                    // Maximum file size
                    rlim.rlim_cur = rlim_val;
                    rlim.rlim_max = rlim_val;
                    setrlimit(RLIMIT_FSIZE, &rlim);
                    break;
                case 't':
                    // Maximum CPU time
                    rlim.rlim_cur = rlim_val;
                    rlim.rlim_max = rlim_val;
                    setrlimit(RLIMIT_CPU, &rlim);
                    break;
                case '?':
                    printf("Unknown option: %c\n", optopt);
                    break;
                default:
                    break;
            }
        } else {
            printf("Invalid argument for option %c\n", opt);
        }
    }
}

int main(int argc, char **argv) {
    resourceLims(argv);
    return 0;
}
