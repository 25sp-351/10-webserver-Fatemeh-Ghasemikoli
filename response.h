#ifndef RESPONSE_H
#define RESPONSE_H

int send_http_response(int client_sock, const char* content_type,
                       const char* body, int keep_alive);

#endif
