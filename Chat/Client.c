#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void){
    int sockfd;
    struct sockaddr_in serverAddr;

    pid_t pid;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    printf("Enter your name: ");
    char inputName[1024];
    fgets(inputName, sizeof(inputName), stdin);
    char sendName[2048] = "User Connected: ";
    strcat(sendName, inputName);
    sendto(sockfd, sendName, sizeof(sendName), 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    printf("Connected to server\n");
    if((pid = fork()) == 0){
        while (1) {
            char input[1024];
            fgets(input, sizeof(input), stdin);

            sendto(sockfd, input, sizeof(input), 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
        }
    }
    while (1){
        char buffer[1024];
        int n = recvfrom(sockfd, buffer, 1024, 0, NULL, NULL);
        buffer[n] = 0;
        printf("%s", buffer);
    }

}
