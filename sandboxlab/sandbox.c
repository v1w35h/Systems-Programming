#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

void parse_commands(char *input);
void execCmd(char **array_o_inputs, bool read, bool write, bool append, char *read_file, char *write_file, char *append_file);
void writeRedirect(char *input);
void readRedirect(char *input);
void appendRedirect(char *input);

void start_sandbox(void) {
  // getenv to get user and home to print command line
  const char *user = getenv("USER");
  const char *home = getenv("HOME");
  // input line and current working directory
  char input[1024];
  char cwd[1024];

  // loop until exit
  while (1) {
    getcwd(cwd, sizeof(cwd));

    // check if home is in the cwd
    if (strncmp(cwd, home, strlen(home)) == 0) {
      // print the cwd without the home
      printf("%s@sandbox:~%s> ", user, cwd + strlen(home));
    } else {
      printf("%s@sandbox:%s> ", user, cwd);
    }

    fgets(input, sizeof(input), stdin);
    // take newline out of input and replace with terminator
    size_t newline = strcspn(input, "\n");
    input[newline] = '\0';

    // check for exit and break
    if (strncmp(input, "exit", 4) == 0) {
      return;
    } else {
      parse_commands(input);
    }
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void parse_commands(char *input) {
  int token_count = 0;
  char *token;
  char *input_arr[1024];
  // flags and file input for my convoluted redirection method
  bool read = false;
  bool write = false;
  bool append = false;
  char *read_file = NULL;
  char *write_file = NULL;
  char *append_file = NULL;

  // get the first input and put it into the array of input strings
  token = strtok(input, " ");
  input_arr[token_count] = token;

  // This is a loop to get all arguments that I got from geeksforgeeks
  while (token != NULL) {
    if (strncmp(token, "<", 1) == 0) {
      printf("< read file\n");
      read = true;
      read_file = token+1;

    } else if (strncmp(token, ">>", 2) == 0) {
      printf(">> append file\n");
      append = true;
      append_file = token+1;

    } else if (strncmp(token, ">", 1) == 0) {
      printf("> write file\n");
      write = true;
      write_file = token+1;

    } else if (strncmp(token,"$", 1) == 0){
      // expand the environment variable by storing it in the input array with getenv without the $ sign
      // search for the / character
      char* slash = strstr(token, "/");
      token += strlen("$");
      char buf[1024];

      if(slash != NULL){
        // printf("%s\n",slash);
        char *expand;
        int j=0;
        for(int i =0; i< strlen(token); i++){
          if(token[i] == '/'){
            break;
          }
          j++;
        }
        expand = strndup(token, j);
        char *expandedEnv = getenv(expand);

        snprintf(buf, sizeof(buf), "%s/%s", expandedEnv, slash+1);
        // had to strdup here or else, i would have a memory error when a bad 
        // directory is input
        input_arr[token_count] = strdup(buf); 
        free(expand);
        // free(buf);
      } else{
        //if there isn't a / just set the environment variable
        input_arr[token_count] = getenv(token);
      }
    }

    token = strtok(NULL, " ");

    token_count += 1;
    input_arr[token_count] = token;
  }
  if(read){
    printf("%s\n", read_file);
  }
  // printf("%d\n", token_count);
  if (strcmp(input_arr[0], "cd") == 0 && token_count == 1){
    chdir(getenv("HOME"));
  } else if (strcmp(input_arr[0], "cd") == 0 && token_count == 2) {
    if (chdir(input_arr[1]) != 0) {
      printf("cd: no such file or directory: %s\n", input_arr[1]);
    }
  } else {
    execCmd(input_arr, read, write, append, read_file, write_file, append_file);
    // writeRedirect(writeToken);
  }
  return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void execCmd(char **array_o_inputs, bool read, bool write, bool append, char *read_file, char *write_file, char *append_file) {
  pid_t pid;

  pid = fork();

  if (pid == 0) {
    if(write){
      // printf("TODO: execute the write redirection\n");
      writeRedirect(write_file);
    } 
    if(read){
      readRedirect(read_file);
    } 
    if(append){
      appendRedirect(append_file);
    }
    execvp(array_o_inputs[0], array_o_inputs);
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  }
  else{
    return;
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void writeRedirect(char *input){
  int fd;

  fd = open(input, O_WRONLY | O_CREAT | O_TRUNC , 666);
  if(fd < 0){
    printf("%s failed to open\n", input);
    return;
  }

  dup2(fd, 1);
  close(fd);
}

void readRedirect(char *input){
  int fd;

  fd = open(input, O_RDONLY , 666);
  if(fd < 0){
    printf("%s failed to open\n", input);
    return;
  }

  dup2(fd, 0);
  close(fd);
}

void appendRedirect(char *input){
  int fd;

  fd = open(input, O_WRONLY | O_CREAT | O_APPEND , 666);
  if(fd < 0){
    printf("%s failed to open\n", input);
    return;
  }

  dup2(fd, 1);
  close(fd);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main() {
  start_sandbox();

  return 0;
}