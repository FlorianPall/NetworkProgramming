#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char **argv){
   int listensocket, acceptsocket;
   struct sockaddr_in server;

   listensocket = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server, sizeof(server));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    bind(listensocket, (struct sockaddr*)&server, sizeof(server));

    listen(listensocket, 10);

    while(1){
        acceptsocket = accept(listensocket, (struct sockaddr*)NULL, NULL);
        write(acceptsocket, "HALLO", sizeof("HALLO"));
    }
}