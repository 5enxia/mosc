#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "err.h"

int send_msg(int fd, char *msg) {
    int len = strlen(msg);
    if (write(fd, msg, len) != len) die("http write() error");
    return len;
}

void http(int sock) {
    int err;

    int len;
    int read_fd;
    char buf[1024];
    char method_name[16];
    char uri_address[256];
    char http_version[64];
    char *uri_file;

    err = read(sock, buf, 1024);
    // if (err <= 0) die("http read() error");
    if (err <= 0) fprintf(stderr, "http read() error.\n");


    sscanf(buf, "%s %s %s", method_name, uri_address, http_version); // Parse Request
    printf("%s %s %s\n", method_name, uri_address, http_version);
    err = strcmp(method_name, "GET");
    if (err != 0) send_msg(sock, "501 Not Implemented"); // POST, UPDATE, DELETE etc...
    else { // GET
        uri_file = uri_address + 1;
        read_fd = open(uri_file, O_RDONLY, 0666);
        if (read_fd == -1) send_msg(sock, "404 Not Found");
        else {
            send_msg(sock, "HTTP/1.0 200 OK\r\n");
            send_msg(sock, "Content-Type: text/html\r\n");
            // send_msg(sock, "Content-Length: 1024\r\n");
            send_msg(sock, "\r\n");
            while ((len = read(read_fd, buf, 1024)) > 0) {
                // if (write(sock, buf, len) != len) die("http response error");
                if (write(sock, buf, len) != len) fprintf(stderr, "http read() error.\n");
            }
        }
        close(read_fd);
    }
}
