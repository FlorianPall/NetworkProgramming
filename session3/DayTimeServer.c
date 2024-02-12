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

int
main(int argc, char **argv)
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(13);	/* daytime server */

    bind(listenfd, (const struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listenfd, BACKLOG);

    for ( ; ; ) {
        connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connfd, buff, strlen(buff));

        close(connfd);
    }
}