#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client_handler.h"

#define DEFAULT_PORT 80
#define BACKLOG 10

int server_port = DEFAULT_PORT;  // Default port

int main(int argc, char* argv[]) {
    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            char* endptr;
            server_port = (int)strtol(argv[i + 1], &endptr, 10);
            if (*endptr != '\0') {
                fprintf(stderr, "Invalid port number: %s\n", argv[i + 1]);
                exit(EXIT_FAILURE);
            }
        }
    }
    // Create a TCP socket (IPv4, stream-based)
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    struct sockaddr_in addr = {0};
    addr.sin_family         = AF_INET;
    addr.sin_port           = htons(server_port);  // Port in network byte order
    addr.sin_addr.s_addr =
        INADDR_ANY;  // Accept connections from any IP address

    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // Start listening for incoming connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Server listening on port %d...\n", server_port);
    // Main server loop: accept and handle incoming client connections
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);
        int* client_sock    = malloc(sizeof(int));
        *client_sock =
            accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
        if (*client_sock < 0) {
            perror("accept failed");
            free(client_sock);
            continue;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_sock);
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}
