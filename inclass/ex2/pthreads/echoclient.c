#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
  struct addrinfo hints;
  struct addrinfo *r, *rp;
  int s;
  int ssock;

  if (argc != 3) {
    printf("Usage: %s <host> <port>\n", argv[0]);
    return -1;
  }

  // not supposed to do this **memset does shallow set
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  s = getaddrinfo(argv[1], argv[2], &hints, &r);
  if (s != 0) {
    printf("getaddrinfo: %s\n", gai_strerror(s));
    return -1;
  }

  for (rp = r; rp != NULL; rp = rp->ai_next) {
    ssock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (ssock == -1) {
      continue;
    }
    if (connect(ssock, rp->ai_addr, rp->ai_addrlen) == 0) {
      break;
    }
  }
  freeaddrinfo(r);
  if (rp == NULL) {
    printf("Could not connect to %s:%s.\n", argv[1], argv[2]);
    return -1;
  }

  char line[256];
  ssize_t bytes;

  while (fgets(line, sizeof(line), stdin) != NULL) {
    bytes = send(ssock, line, strlen(line), 0);
    
    bytes = recv(ssock, line, sizeof(line) - 1, 0);
    if (bytes == 0) {
      break;
    }
    line[bytes] = '\0';
    printf("%s\n", line);
  }
  shutdown(ssock, SHUT_RDWR);
  close(ssock);
}
