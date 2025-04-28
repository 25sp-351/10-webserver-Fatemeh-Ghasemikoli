#include "div_handler.h"

#include <stdio.h>
#include <string.h>

#include "response.h"

// Function to handle the "/div" path
void handle_div(int client_sock, const char* path, bool keep_alive) {
    int a, b;
    if (sscanf(path + 10, "%d/%d", &a, &b) ==
        2) {  // Extract two integers from the path
        // Check if the second integer is zero to avoid division by zero
        char result_str[64];
        if (b == 0) {
            snprintf(result_str, sizeof(result_str),
                     "Error: Division by zero\n");
        } else {
            snprintf(result_str, sizeof(result_str), "Result: %d\n", a / b);
        }
        send_http_response(client_sock, "text/plain", result_str, keep_alive);
    }
}
