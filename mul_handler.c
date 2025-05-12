#include "mul_handler.h"

#include <stdio.h>
#include <string.h>

#include "response.h"

// Function to handle the "/mul" path
// This function extracts two integers from the path and returns their product
void handle_mul(int client_sock, const char* path, bool keep_alive) {
    int a, b;
    if (sscanf(path + 10, "%d/%d", &a, &b) == 2) {
        char result_str[64];
        snprintf(result_str, sizeof(result_str), "Result: %d\n", a * b);
        send_http_response(client_sock, "text/plain", result_str, keep_alive);
    }
}
