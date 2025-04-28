#include "request.h"

#include <stdio.h>
#include <string.h>

// Function to parse a raw HTTP request line into an http_request_t struct
int parse_http_request(const char* raw, http_request_t* request) {
    // Try to scan three components: method, path, and HTTP version
    int scanned = sscanf(raw, "%s %s %s", request->method, request->path,
                         request->version);
    if (scanned != 3) {
        fprintf(stderr, "Raw request that failed: %s\n", raw);
        return -1;  // Malformed
    }
    // Check if the method is GET (only GET requests are supported)
    if (strcmp(request->method, "GET") != 0)
        return -2;  // Only GET supported

    // Strictly require HTTP/1.1
    if (strcmp(request->version, "HTTP/1.1") != 0)
        return -3;

    return 0;
}
