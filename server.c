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

    int serverSocket, clientSocket;
    struct sockaddr_in serverSocketAddress, clientSocketAddress;
    unsigned short serverPort;
    unsigned int clientAddressLength;

    if (argc != 2) die("Argument count is mismatch");

    serverPort = (unsigned short)atoi(argv[1]);
    if (!serverPort) die("Invalid port number");

    serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket < 0) die("socket() failed");

    memset(&serverSocketAddress, 0, sizeof(serverSocketAddress));
    serverSocketAddress.sin_family = AF_INET;
    serverSocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverSocketAddress.sin_port = htons(serverPort);
    
    err = bind(serverSocket, (struct sockaddr *)&serverSocketAddress, sizeof(serverSocketAddress));
    if (err < 0) die("bind() failed");

    err = listen(serverSocket, LIMIT_OF_QUEUE);
    if (err < 0) die("listen() failed");

    while (1)
    {
        clientAddressLength = sizeof(clientSocketAddress);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientSocketAddress, &clientAddressLength); 
        if (clientSocket < 0) die("accept() failed");

        printf("Connected from %s\n", inet_ntoa(clientSocketAddress.sin_addr));
        close(clientSocket);
    }

    return EXIT_SUCCESS;
}
