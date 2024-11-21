/* Viwesh Gupta
Sandbox lab
making my own shell
this lab was fun at first, but then I started really hating it towards the end
some resources i used:
https://www.geeksforgeeks.org/making-linux-shell-c/
https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/

I hate to give up but its 4:30am and I need to sleep 

*/

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector.h>

void prompt(void);
void parse_commands(char *input, char **argv, Vector *vec);
void execCmd(char **input_arr, char **argv, char *read_file, char *write_file,
             char *append_file, bool job, Vector *vec);
void writeRedirect(char *input);
void readRedirect(char *input);
void appendRedirect(char *input);

void resourceLims(char **argv);

//  prints the users specific terminal line
void prompt(void) {
  // getenv to get user and home to print command line
  const char *user = getenv("USER");
  const char *home = getenv("HOME");
  // current working directory
  char cwd[1024];

  getcwd(cwd, sizeof(cwd));

  // check if home is in the cwd
  if (strncmp(cwd, home, strlen(home)) == 0) {
    // print the cwd without the home
    printf("%s@sandbox:~%s> ", user, cwd + strlen(home));
  } else {
    printf("%s@sandbox:%s> ", user, cwd);
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void parse_commands(char *input, char **argv, Vector *vec) {
  unsigned int token_count = 0;
  char *token;
  // hold all the given input in this array
  char *input_arr[1024];
  // flag and file inputs for my convoluted redirection method
  char *read_file = NULL;
  char *write_file = NULL;
  char *append_file = NULL;

  // get the first input to start looping through the input
  token = strtok(input, " ");

  // This is a loop to get all arguments that I got from geeksforgeeks
  while (token != NULL) {
    // look for specific chars from tokens
    if (strncmp(token, "<", 1) == 0) {
      read_file = token + 1;
      // printf("< read file: %s\n", read_file);
      token = strtok(NULL, " ");

    } else if (strncmp(token, ">>", 2) == 0) {
      append_file = token + 2;
      // printf(">> append file: %s\n", append_file);
      token = strtok(NULL, " ");

    } else if (strncmp(token, ">", 1) == 0) {
      write_file = token + 1;
      // printf("> write file: %s\n", write_file);
      token = strtok(NULL, " ");

    } else if (strncmp(token, "$", 1) == 0) {
      // expand the environment variable by storing it in the input array with
      // getenv without the $ sign search for the / character
      char *slash = strstr(token, "/");
      token += 1;
      char buf[1024];

      if (slash != NULL) {
        // printf("%s\n",slash);
        char *expand;
        int j = 0;
        for (int i = 0; i < (int)strlen(token); i++) {
          if (token[i] == '/') {
            break;
          }
          j++;
        }
        expand = strndup(token, j);
        char *expandedEnv = getenv(expand);

        // create the actual directory fully using snprintf
        snprintf(buf, sizeof(buf), "%s/%s", expandedEnv, slash + 1);
        // had to strdup here or else, i would have a memory error when a bad
        // directory is input, but I don't understand why freeing causes an
        // error
        token = strdup(buf);
        free(expand);
      } else {
        // if there isn't a / just set the environment variable
        token = getenv(token);
      }
    }

    input_arr[token_count] = token;
    token_count += 1;
    token = strtok(NULL, " ");
  }
  // to prevent extra character from being added to input_arr
  input_arr[token_count] = NULL;

  // for jobs set a flag
  bool job = false;
  // for some reason I can't use token_count to look for ampersand so i added
  // this counter instead
  int i = 0;
  while (input_arr[i] != NULL) {
    i++;
  }
  // look for ampersand
  if (i > 1 && strcmp(input_arr[i - 1], "&") == 0) {
    job = true;
    // remove the & from the input array
    input_arr[i - 1] = NULL;
  }

  // do nothing when you try to just <input.txt
  if ((input_arr[0] == read_file) && token_count == 1) {
    return;
  } else if (strcmp(input_arr[0], "cd") == 0 && token_count == 1) {
    // change dir to home from no extra inputs or change into specified dir
    chdir(getenv("HOME"));
  } else if (strcmp(input_arr[0], "cd") == 0 && token_count == 2) {
    if (chdir(input_arr[1]) != 0) {
      printf("cd: no such file or directory: %s\n", input_arr[1]);
    }
  }
  else {
    // send the input array , as well as this other cluster of args that I've been bouncing to other functions
    // into the execCmd
    execCmd(input_arr, argv, read_file, write_file, append_file, job, vec);
  }
  return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void execCmd(char **input_arr, char **argv, char *read_file, char *write_file,
             char *append_file, bool job, Vector *vec) {
  pid_t *pid;
  // fork the process to execute commands 
  pid = fork();
  int status;

  if (pid == 0) {
    // check if the file is NULL or not
    if (read_file) {
      readRedirect(read_file);
    }
    if (append_file) {
      appendRedirect(append_file);
    }
    if (write_file) {
      writeRedirect(write_file);
    }
    resourceLims(argv);

    // call execvp on the inputs 
    execvp(input_arr[0], input_arr);

  } else if (pid > 0) {
    // if there was an & hold the  pids for those jobs in the passed in vector
    if (job) {
      vector_push(vec, (int64_t)pid);

      waitpid(pid, &status, WNOHANG);
    } else {
      // or waitpid as if it should execute normally  like in fork_exec.c
      waitpid(pid, &status, 0);
      if (WIFEXITED(status)) {
        if (WEXITSTATUS(status) != 0) {
          printf("Execution failed with code %u.\n", WEXITSTATUS(status));
        }
      }
    }
  } else {
    return;
  }

  return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// redirection functions
// open the file with specified flags and dup2 to the write file descriptors
void writeRedirect(char *input) {
  int fd;

  fd = open(input, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (fd < 0) {
    printf("%s failed to open\n", input);
    return;
  }

  dup2(fd, STDOUT_FILENO);
  close(fd);
  input = NULL;
}

void readRedirect(char *input) {
  int fd;

  fd = open(input, O_RDONLY, 0666);
  if (fd < 0) {
    printf("%s failed to open\n", input);
    return;
  }

  dup2(fd, STDIN_FILENO);
  close(fd);
  input = NULL;
}

void appendRedirect(char *input) {
  int fd;

  fd = open(input, O_WRONLY | O_CREAT | O_APPEND, 0666);
  if (fd < 0) {
    printf("%s failed to open\n", input);
    return;
  }

  dup2(fd, STDOUT_FILENO);
  close(fd);
  input = NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void resourceLims(char **argv) {
  struct rlimit rlim;
  int argc = 0;

  // count how many args
  while (argv[argc] != NULL) {
    argc++; // haha it says c++
  }
  // set soft and hard limits
  // maximum processes -p
  setrlimit(RLIMIT_NPROC, &(struct rlimit){256, 256});
  // maximum data -d
  setrlimit(RLIMIT_DATA, &(struct rlimit){1 << 30, 1 << 30});
  // maximum stack -s
  setrlimit(RLIMIT_STACK, &(struct rlimit){1 << 30, 1 << 30});
  // maximum fd open -n
  setrlimit(RLIMIT_NOFILE, &(struct rlimit){256, 256});
  // maximum file size -f
  setrlimit(RLIMIT_FSIZE, &(struct rlimit){1 << 30, 1 << 30});
  // maximum CPU time -t
  setrlimit(RLIMIT_CPU, &(struct rlimit){1 << 30, 1 << 30});

  int opt;
  int rlim_val;

  while ((opt = getopt(argc, argv, "p:d:s:n:f:t")) != -1) {
    // take the optarg and set those values
    if (optarg != NULL && sscanf(optarg, "%d", &rlim_val) == 1) {
      switch (opt) {
      // change the limits based on the option
      case 'p':
        rlim.rlim_cur = rlim_val;
        rlim.rlim_max = rlim_val;
        setrlimit(RLIMIT_NPROC, &rlim);
        break;
      case 'd':
        rlim.rlim_cur = rlim_val;
        rlim.rlim_max = rlim_val;
        setrlimit(RLIMIT_DATA, &rlim);
        break;
      case 's':
        rlim.rlim_cur = rlim_val;
        rlim.rlim_max = rlim_val;
        setrlimit(RLIMIT_STACK, &rlim);
        break;
      case 'n':
        rlim.rlim_cur = rlim_val;
        rlim.rlim_max = rlim_val;
        setrlimit(RLIMIT_NOFILE, &rlim);
        break;
      case 'f':
        rlim.rlim_cur = rlim_val;
        rlim.rlim_max = rlim_val;
        setrlimit(RLIMIT_FSIZE, &rlim);
        break;
      case 't':
        rlim.rlim_cur = rlim_val;
        rlim.rlim_max = rlim_val;
        setrlimit(RLIMIT_CPU, &rlim);
        break;
      case '?':
        printf("Unknown option: %c\n", optopt);
        break;
      }
    } else {
      return;
    }
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main(int argc, char *argv[]) {
  char input[1024];
  // vector for storing the pids  & val to get the number  from the vector
  Vector *vec = vector_new();
  int64_t val;

  while (1) {
    prompt();
    // read in  input
    fgets(input, sizeof(input), stdin);
    // take newline out of input and replace with terminator
    size_t newline = strcspn(input, "\n");
    input[newline] = '\0';

    // check for exit and break
    if (strncmp(input, "exit", 4) == 0) {
      break;
    }
    // print jobs
    else if (strncmp(input, "jobs", 4) == 0) {
      if (vector_size(vec) == 0) {
        printf("%d jobs.\n", vector_size(vec));
      } else if (vector_size(vec) == 1) {
        printf("%d job.\n", vector_size(vec));
        vector_get(vec, vector_size(vec) - 1, &val);
        printf("  %d  -  \n", val);
      } else {
        printf("%d jobs.\n", vector_size(vec));
        for (int i = 0; i < vector_size(vec); i++) {
          vector_get(vec, i, &val);
          printf("  %d  -  \n", val);
        }
      }
    }

    else {
      // start parsing the commands
      parse_commands(input, argv, vec);
      // passing around argv to every function until it gets to resourceLims
      // there's definitly a better way to do this but my slow brain couldn't
      // find it
    }
  }
  // free the vector
  vector_free(vec);
  return 0;
}