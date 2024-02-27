
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void AddClient(struct sockaddr_in* clientAddr, int clientLen);

struct Client{
    struct sockaddr_in clientAddr;
    int clientLen;
};

struct Client clients[10];

int main(void){
    int sockfd;
    struct sockaddr_in serverAddr;



    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(8080);

    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    while (1){

        struct sockaddr_in from;
        socklen_t fromlen = sizeof(from);

        char buffer[1024];
        int n = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&from, &fromlen);

        AddClient(&from, fromlen);

        for (int i = 0; i < 10; i++){
            if (clients[i].clientAddr.sin_port != from.sin_port && clients[i].clientLen != 0){
                sendto(sockfd, buffer, n, 0, (struct sockaddr*)&clients[i].clientAddr, clients[i].clientLen);
            }
        }
    }
}

void AddClient(struct sockaddr_in* clientAddr, int clientLen){
    for (int i = 0; i < 10; i++){
        if (clients[i].clientAddr.sin_addr.s_addr == clientAddr -> sin_addr.s_addr && clients[i].clientAddr.sin_port == clientAddr -> sin_port){
            break;
        }
        else if(clients[i].clientLen == 0){
            clients[i].clientAddr = *clientAddr;
            clients[i].clientLen = clientLen;
            break;
        }
    }

}
