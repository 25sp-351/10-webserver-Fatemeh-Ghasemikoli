#ifndef REQUEST_H
#define REQUEST_H

typedef struct {
    char method[8];
    char path[1024];
    char version[16];
} http_request_t;

int parse_http_request(const char* raw, http_request_t* request);

#endif
