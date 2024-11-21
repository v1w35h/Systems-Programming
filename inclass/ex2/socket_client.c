#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    struct addrinfo *res, *rp;
    struct addrinfo hints = {};
    int errcode;
    int sock;


    if(argc != 3){
        printf("usage: ./client <host> <port>")
    }

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // build linked list
    errcode = getaddrinfo(argv[1], argv[2], &hints, &rp);
    if(errcode != 0) {
        printf("getaddrinfo: %s\n", gai_strerror(errcode));
        return -1;
    }

    // res is head in the linked list
    for(rp = res; rp != NULL; rp = rp->ai_next) {
        // make a connection
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sock == -1){
            continue;
        }
        // create a connection here
        if (connect(sock, rp->ai_addr, rp->ai_addrlen) != 0) {
            close(sock);
        }
        else{
            break;
        }
    }

    freeaddrinfo(res);

    if(rp == NULL){
        printf("Could not connect to  server %s: %s\n", "web.eecs.utk.edu", "80");
        return -1;
    }

    // for(;;){
    //     char buffer[256];
    //     ssize_t bytes;

    //     bytes = recv(sock, buffer, sizeof(buffer));
    //     if(bytes == 0) {
    //         break;
    //     }
    // }

    char line[256];
    ssize_t  bytes;

    while(fgets(line,sizeof(line) - 1, stdin != NULL)) {
        bytes = send(sock, line, strlen(line), 0);

        bytes = recv(sock, line, sizeof(line) - 1, 0);
        if(bytes == 0){
            break;
        }
        line[bytes] = '\0';

        printf("%s\n", line);
    }

    close(sock);
}