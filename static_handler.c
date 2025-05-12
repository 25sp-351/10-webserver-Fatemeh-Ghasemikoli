#include "static_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "response.h"
// Function to serve static files to the client
void handle_static(int client_sock, const char* path, bool keep_alive) {
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), ".%s", path);

    FILE* fp = fopen(filepath, "rb");
    if (!fp) {
        send_http_response(client_sock, "text/plain", "404 Not Found\n",
                           keep_alive);
        return;
    }
    // Move to the end of file to determine its size

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    // Move back to the beginning of the file
    rewind(fp);

    // Allocate memory to hold the entire file
    char* filedata = malloc(filesize);
    fread(filedata, 1, filesize, fp);
    fclose(fp);

    // Determine content type based on file extension
    const char* content_type = "application/octet-stream";
    if (strstr(filepath, ".html"))
        content_type = "text/html";
    else if (strstr(filepath, ".txt"))
        content_type = "text/plain";
    else if (strstr(filepath, ".png"))
        content_type = "image/png";
    else if (strstr(filepath, ".jpg") || strstr(filepath, ".jpeg"))
        content_type = "image/jpeg";
    else if (strstr(filepath, ".css"))
        content_type = "text/css";
    else if (strstr(filepath, ".js"))
        content_type = "application/javascript";

    // Build the HTTP response header
    char header[1024];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "Connection: %s\r\n"
             "\r\n",
             content_type, filesize, keep_alive ? "keep-alive" : "close");

    // Send the HTTP headers first
    send(client_sock, header, strlen(header), 0);

    // Then send the file content
    send(client_sock, filedata, filesize, 0);
    free(filedata);
}
