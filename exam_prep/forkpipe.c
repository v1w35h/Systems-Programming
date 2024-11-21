#include <stdio.h>
#include <string.h>
// #include <sys/wait.h> // for waitpid and WXXX() macros
#include <unistd.h>
int main(int argc, char *argv[]) {
  char buffer[17];
  int fds[2];
  pid_t pid;
  pipe(fds);
  // fds[0] is the "read" end
  // fds[1] is the "write" end.
  // These are simplexes, meaning one-way direction.
  // Since fork() copies the file descriptor table, the child
  // and the parent share these two pipes, but to avoid synchronization
  // issues, we have to close off the sections not being used by the
  // child/parent.
  // In this case, parent reads, child writes.
  pid = fork();
  if (pid == 0) {
    // Since the child writes, we need to close fds[0] (read end)
    close(fds[0]);
    // All that is open for the child is fds[1], which will be connected
    // to the parent via fds[0].
    do {
      printf("Enter command ('quit' to quit): ");
      if (1 == scanf("%16s", buffer)) {
        if (!strcmp(buffer, "quit")) {
          break;
        }
        write(fds[1], buffer, strlen(buffer));
      }
    } while (1);
    close(fds[1]);
  } else if (pid > 0) {
    ssize_t bytes;
    int status;
    // Only the child will handle input, so close standard input from the
    // parent.close(STDIN_FILENO);
    // Since the parent reads, we need to close fds[1] (write end)
    close(fds[1]);
    do {
      bytes = read(fds[0], buffer, 16);
      // When the other side of the pipe has been closed, read will return 0
      // which indicates we can no longer read. It isn't an error, but we
      // still must handle errors, which is why we compare bytes <= 0 rather
      // than just 0 or just < 0.
      if (bytes <= 0 || !strncmp(buffer, "quit", 4)) {
        break;
      }
      buffer[bytes] = '\0';
      printf("Client wrote: '%s'\n", buffer);
    } while (1);
    close(fds[0]);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
      printf("Child exited successfully with code %d.\n", WEXITSTATUS(status));
    } else {
      printf("Child exited abnormally!\n");
    }
  }
}
