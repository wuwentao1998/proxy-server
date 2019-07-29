#ifndef HTTP_H
#define HTTP_H

#include "robust_IO.h"
#include "macro.h"

void deal(int client);
void ignore_remaining_header(rio_t* rio);
int parse_URL(char* URL, char* URI, char* host, int* port_ptr);
void build_http_header(char* http_header, char* host, char* filename);
void server_error(int fd, char* casue, char* error_num,
                char* short_message, char* long_message);

#endif