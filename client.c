#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MESSAGE_SIZE 32
#define MAX_MESSAGE_SIZE 1024
#define BUFSIZE (MESSAGE_SIZE + 1)

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[])
{
    int err;

    int sock;
    struct sockaddr_in serverSocketAddress;
    unsigned short serverPortNumber;
    char recivedBuffer[BUFSIZE];
    int receivedBufferByteSize, totalReceivedBufferByteSize;

    if (argc != 3) die("Argument count is mismatch");

    memset(&serverSocketAddress, 0, sizeof(serverSocketAddress));

    serverSocketAddress.sin_family = AF_INET;

    err = inet_aton(argv[1], &serverSocketAddress.sin_addr);
    if (err == 0) die("Invalid IP Address.\n");

    serverPortNumber = (unsigned short)atoi(argv[2]);
    if (serverPortNumber == 0) die("Invalid Port Number.\n");
    serverSocketAddress.sin_port = htons(serverPortNumber);

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (err < 0) die("socket() failed.");

    err = connect(sock, (struct sockaddr*) &serverSocketAddress, sizeof(serverSocketAddress));
    if (err < 0) die("connect() failed.");

    printf("Connect tt %s\n", inet_ntoa(serverSocketAddress.sin_addr));

    totalReceivedBufferByteSize = 0;
    while (totalReceivedBufferByteSize < MAX_MESSAGE_SIZE)
    {
        receivedBufferByteSize = recv(sock, recivedBuffer, MESSAGE_SIZE, 0);
        if (receivedBufferByteSize > 0) {
            recivedBuffer[receivedBufferByteSize] = '\0';
            printf("%s", recivedBuffer);
            totalReceivedBufferByteSize += receivedBufferByteSize;
        } 
        else if (receivedBufferByteSize == 0) die("ERR_EMPTY_RESPONSE");
        else die("recv() failed.");
    }
    printf("\n");

    close(sock);

    return EXIT_SUCCESS;
}
