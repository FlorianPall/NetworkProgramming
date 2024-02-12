//
// Created by Florian Pallmann on 08.02.24.
//
#include <strings.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>

#define MAXLINE 4096
#define BACKLOG 10

void ErrorHandling(char* message);
struct sockaddr_in GetIP(char* adress, char* port);

int
main(int argc, char **argv)
{
    int fd;
    int n;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];

    if (argc != 3) {
        ErrorHandling("usage: <IPaddress> <Port>");
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr = GetIP(argv[1], argv[2]);

    if (connect(fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        ErrorHandling("connect error");
    }
    printf("Connected to %s:%s\n", argv[1], argv[2]);
    while ( (n = read(fd, buff, MAXLINE)) > 0) {
        buff[n] = 0;        /* null terminate */
        printf("%s\n",buff);
    }
    close(fd);
    exit(0);
}


struct sockaddr_in GetIP(char* adress, char* port){
    struct addrinfo hints, *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4

    int status = getaddrinfo(adress, port, &hints, &result);
    if(status != 0) {
        ErrorHandling("getaddrinfo() error");
    }

    struct sockaddr_in *addr = (struct sockaddr_in *)result->ai_addr;
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr->sin_addr), ipstr, INET_ADDRSTRLEN);

    return *addr;
}


void ErrorHandling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(-1);
}