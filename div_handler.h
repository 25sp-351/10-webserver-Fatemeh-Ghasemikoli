#ifndef DIV_HANDLER_H
#define DIV_HANDLER_H

#include <stdbool.h>

void handle_div(int client_sock, const char* path, bool keep_alive);

#endif
