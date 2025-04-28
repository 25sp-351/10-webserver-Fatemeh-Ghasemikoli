#ifndef ADD_HANDLER_H
#define ADD_HANDLER_H

#include <stdbool.h>

void handle_add(int client_sock, const char* path, bool keep_alive);

#endif
