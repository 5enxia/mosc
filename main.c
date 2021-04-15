#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LIMIT_OF_QUEUE 5

int main(int argc, const char *argv[])
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverSocketAddress, clientSocketAddress;
    unsigned short serverPortNumber;

    if (argc != 2) {
        fprintf(stderr, "ERROR: argument count is mismatch\n");
        exit(EXIT_FAILURE);
    }
}
