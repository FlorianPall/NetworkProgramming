//
// Created by Florian Pallmann on 29.01.24.
//

#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[]) {

    struct addrinfo hints, *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4

    int status = getaddrinfo("localhost", "80", &hints, &result);
    if(status != 0) {
        printf("Error: %s\n", gai_strerror(status));
        return 1;
    }

    struct sockaddr_in *addr = (struct sockaddr_in *)result->ai_addr;
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr->sin_addr), ipstr, INET_ADDRSTRLEN);
    printf("%s\n", ipstr);

    freeaddrinfo(result);

    return 0;
}