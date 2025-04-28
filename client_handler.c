#include "client_handler.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "add_handler.h"
#include "div_handler.h"
#include "mul_handler.h"
#include "request.h"
#include "response.h"
#include "static_handler.h"

// Function to handle client requests
void* handle_client(void* client_sock_ptr) {
    int client_sock = *(int*)client_sock_ptr;  // Dereference the pointer
    free(client_sock_ptr);                     // Free the allocated memory

    char buffer[4096];
    ssize_t received;
    size_t buffer_used = 0;
    // Buffer to store the received data
    while ((received = recv(client_sock, buffer + buffer_used,
                            sizeof(buffer) - buffer_used - 1, 0)) > 0) {
        buffer_used += received;
        buffer[buffer_used] = '\0';
        // Null-terminate the buffer
        char* request_end = strstr(buffer, "\r\n\r\n");
        if (!request_end)
            continue;  // Wait for full request

        // Temporarily null-terminate at the end of the first request
        size_t request_length  = request_end - buffer + 4;
        buffer[request_length] = '\0';

        fprintf(stdout, "---- Received Request ----\n%s\n", buffer);

        http_request_t req;  // Initialize the request structure
        int result = parse_http_request(buffer, &req);  // Parse the request
        if (result == 0) {
            fprintf(stdout, "Method: %s\nPath: %s\nVersion: %s\n", req.method,
                    req.path, req.version);
            // Check if the request is a GET request
            bool close_connection = strstr(buffer, "Connection: close") != NULL;

            if (strncmp(req.path, "/calc/add/", 10) == 0) {
                handle_add(client_sock, req.path, !close_connection);
            } else if (strncmp(req.path, "/calc/mul/", 10) == 0) {
                handle_mul(client_sock, req.path, !close_connection);
            } else if (strncmp(req.path, "/calc/div/", 10) == 0) {
                handle_div(client_sock, req.path, !close_connection);
            } else if (strncmp(req.path, "/static/", 8) == 0) {
                handle_static(client_sock, req.path, !close_connection);
            } else if (strncmp(req.path, "/sleep/", 7) == 0) {
                int seconds;
                if (sscanf(req.path + 7, "%d", &seconds) == 1 && seconds >= 0 &&
                    seconds <= 30) {
                    fprintf(stdout, "[INFO] Sleeping for %d seconds...\n",
                            seconds);
                    sleep(seconds);
                    char result_str[128];
                    snprintf(result_str, sizeof(result_str),
                             "Done sleeping for %d seconds.\n", seconds);
                    send_http_response(client_sock, "text/plain", result_str,
                                       !close_connection);
                } else {
                    send_http_response(client_sock, "text/plain",
                                       "Invalid sleep duration.\n",
                                       !close_connection);
                }
            } else {
                send_http_response(client_sock, "text/plain", "404 Not Found\n",
                                   !close_connection);
            }

            if (close_connection)
                break;
        } else {
            fprintf(stderr, "Invalid request (%d)\n", result);
            break;
        }

        // Move remaining buffer to the front for next request
        memmove(buffer, buffer + request_length, buffer_used - request_length);
        buffer_used -= request_length;
    }

    close(client_sock);
    return NULL;
}
