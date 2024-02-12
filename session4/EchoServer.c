//
// Created by Florian Pallmann on 08.02.24.
//
#include <strings.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

#define BACKLOG 10
#define MAXLINE 4096

int main(int argc, char **argv) {
    pid_t pid;
    int procs = 0;

    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    ssize_t n;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(7);   // Port 7 für Echo-Dienst

    bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listenfd, BACKLOG);

    for (;;) {
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
            procs++;
            pid = fork();
        if(pid == 0) {
            printf("%d\n", procs);
            while ((n = read(connfd, buff, MAXLINE)) > 0) {
                write(connfd, buff, n); // Zurückschicken, was empfangen wurde
            }

            if (n < 0) {
                perror("read error");
            }

            close(connfd);
            procs--;
            printf("%d\n", procs);
        }
    }
}