#include "response.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
// Function to send an HTTP response to a client
int send_http_response(int client_sock, const char* content_type,
                       const char* body, int keep_alive) {
    char header[1024];
    int content_length = strlen(body);
    // Create the HTTP response header with dynamic fields
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n"
             "Connection: %s\r\n"
             "\r\n",
             content_type, content_length, keep_alive ? "keep-alive" : "close");
    // Send the HTTP response header to the client
    send(client_sock, header, strlen(header), 0);
    // Send the body (actual content) to the client
    send(client_sock, body, content_length, 0);
    return 0;
}
