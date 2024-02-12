#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BACKLOG 100
#define MAXLINE 4096
#define MAX_PATH_LENGTH 100
#define PORT 80

void ErrorHandling(const char* message);

int main(int argc, char **argv) {
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];

    // Create socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        ErrorHandling("socket error");
    }

    // Initialize server address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT); // Use a port number above 1024

    // Bind socket
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        ErrorHandling("bind error");
    }

    // Listen for connections
    if (listen(listenfd, BACKLOG) < 0) {
        ErrorHandling("listen error");
    }
    else {
        printf("Server listening on port %d\n", PORT);
    }

    // Accept connections and handle requests
    for (;;) {
        FILE *file;

        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
        if (connfd < 0) {
            ErrorHandling("accept error");
        }

        ssize_t n = read(connfd, buff, MAXLINE);
        if (n < 0) {
            ErrorHandling("read error");
        }

        // Extract file path from request
        char file_path[MAX_PATH_LENGTH];
        if (sscanf(buff, "GET %s ", file_path) < 0) {
            ErrorHandling("GET error");
        }

        // Construct full path to requested resource
        char resource[MAX_PATH_LENGTH + 5]; // ".html\0"
        snprintf(resource, sizeof(resource), "./res%s.html", file_path);

        // Open requested file
        if((file = fopen(resource, "r")) == NULL){
            dprintf(connfd, "HTTP/1.1 404 Not Found\nContent-Type: text/html; charset=UTF-8\n\n");
            dprintf(connfd, "<html><body><h1>404 Not Found</h1></body></html>");
            close(connfd);
            continue;
        }

        // Send HTTP response header
        dprintf(connfd, "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\n\n");

        // Send file content
        while (fgets(buff, MAXLINE, file) != NULL) {
            write(connfd, buff, strlen(buff));
        }

        // Close file and connection
        fclose(file);
        close(connfd);
    }

    return 0;
}

void ErrorHandling(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}