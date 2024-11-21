#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <string.h> // for seeting sun_path
#include <netdb.h>

static char *reverse(char *str);

int main(int argc, char *argv[]) {
    struct addrinfo *rp, *res;
    struct addrinfo hints = {};
    int errcode;
    int sock;
    int cli_sock;

    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        return -1;
    }
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    errcode = getaddrinfo(NULL, argv[1], &hints, &res);
    if(errcode != 0) {
        printf("getaddrinfo: %s\n", gai_strerror(errcode));
        return -1;
    }

    for(rp = res; rp != NULL; rp = rp->ai_next) {
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sock == -1) {
            continue;
        }
    }
    freeaddrinfo(res);
    if(rp == NULL){
        printf("unable to bind server to port %s\n", argv[1]);
        return -1;
    }
    listen(sock, 5);

    clisock = accept(sock, NULL, NULL);
    if(clisock <  0){
        perror("accept");
        goto out;
    }
    for(;;){
        char line[256];
        size_t bytes;

        bytes = recv(clisock, line, sizeof(line)- 1, 0);
        if(bytes == 0) {
            break;
        }
        bytes = send(clisock, reverse(line),strlen(line), 0 );
    }
out:
    shutdown(sock, SHUT_RDWR);
    close(sock);
}

static char *reverse(char *str){
    const int len = strlen(str);
    const int mid = len / 2;
    int i;

    for(i = 0; i < mid; i+=1){
        char t = str[i];
        str[i] = str[len -i -1];
        strlen[len -i -1] = t;
    }

}


// #define FILENAME "myunix.sock"

// int main(){
//     int sock;
//     struct sockaddr_un sun;
//     int cli;

//     sock= socket(AF_UNIX, SOCK_STREAM, 0);
//     if(sock < 0){
//         perror("socket");
//         return -1;
//     }

//     // 1) create the socket
//     sun.sun_family = AF_UNIX;
//     // sun.sun_path = 
//     strncpy(sun.sun_path, FILENAME, sizeof(sun.sun_path) - 1);

//     // 2) bind it
//     if(bind(sock, (const struct sockaddr*)&sun, sizeof(sun)) < 0){
//         perror("bind");
//         return -1;
//     }

//     listen(sock,10);

//     // do {} while(1);
//     struct sockaddr_un cli_sun;
//     socklen_t cli_sun_len = sizeof(cli_sun);
//     fprintf(stderr, "Before accept");
//     cli = accept(sock, (struct sockaddr *)&cli_sun, &cli_sun_len);
//     fprintf(stderr, "After accept");

//     close(sock);
//     unlink(FILENAME);
// }

