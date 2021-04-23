#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LIMIT_OF_QUEUE 5

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[])
{
    int err;

    int serverSock, clientSock;
    struct sockaddr_in serverSockAddr, clientSockAddr;
    unsigned short serverPort;
    unsigned int clientAddrLen;

    if (argc != 2) die("Argument count is mismatch");

    serverPort = (unsigned short)atoi(argv[1]);
    if (!serverPort) die("Invalid port number");

    serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock < 0) die("socket() failed");

    memset(&serverSockAddr, 0, sizeof(serverSockAddr));
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverSockAddr.sin_port = htons(serverPort);
    
    err = bind(serverSock, (struct sockaddr *)&serverSockAddr, sizeof(serverSockAddr));
    if (err < 0) die("bind() failed");

    err = listen(serverSock, LIMIT_OF_QUEUE);
    if (err < 0) die("listen() failed");

    while (1)
    {
        clientAddrLen = sizeof(clientSockAddr);
        clientSock = accept(serverSock, (struct sockaddr *)&clientSockAddr, &clientAddrLen); 
        if (clientSock < 0) die("accept() failed");

        printf("Connected from %s\n", inet_ntoa(clientSockAddr.sin_addr));
        close(clientSock);
    }

    return EXIT_SUCCESS;
}
