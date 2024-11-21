// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>

// #define NUM_THREADS 3
// const char *file_paths[NUM_THREADS] = {"../inclass/ex1/ex1.c", "../inclass/ex2/10-14.c", "../filebufferlab/bee_movie_script.txt"};

// void* read_file(void *arg) {
//     int idx = *(int*)arg;
//     FILE *file = fopen(file_paths[idx], "r");
    
//     if (file == NULL) {
//         perror("fopen");
//         pthread_exit(NULL);
//     }

//     char buffer[256];
//     while (fgets(buffer, sizeof(buffer), file)) {
//         printf("Thread %d read: %s", idx, buffer);
//     }

//     fclose(file);
//     free(arg);
//     pthread_exit(NULL);
// }

// int main() {
//     pthread_t threads[NUM_THREADS];

//     for (int i = 0; i < NUM_THREADS; ++i) {
//         int *idx = malloc(sizeof(int));
//         *idx = i;
        
//         if (pthread_create(&threads[i], NULL, read_file, idx) != 0) {
//             perror("pthread_create");
//             return EXIT_FAILURE;
//         }
//     }

//     for (int i = 0; i < NUM_THREADS; ++i) {
//         pthread_join(threads[i], NULL);
//     }

//     return EXIT_SUCCESS;
// }

/* A simple example of using execve() to run the program cat. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
  char *newargv[3];
  int i;

  newargv[0] = "ls";
  newargv[1] = "-l";
  newargv[2] = NULL;

  i = execve("/bin/ls", newargv, envp); // have to include /bin/ls
  perror("exec2: execve failed");
  exit(1);
• Conclusion

}