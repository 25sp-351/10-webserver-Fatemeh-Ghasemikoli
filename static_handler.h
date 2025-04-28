#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H

#include <stdbool.h>

void handle_static(int client_sock, const char* path, bool keep_alive);

#endif
