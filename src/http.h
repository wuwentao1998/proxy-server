#ifndef HTTP_H
#define HTTP_H
#include <stdbool.h>
#include<sys/stat.h>

#include "wrapper.h"
#include "robust_IO.h"
#include "connect.h"

/* http header string */
static const char* _user_agent = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char* _connection = "Connection: close\r\n";
static const char* _proxy = "Proxy-Connection: close\r\n";
static const char* _host_format = "Host: %s\r\n";
static const char* _request_header_format = "GET %s HTTP/1.0\r\n";
static const char* _blank_line = "\r\n";

/* functions */
void deal(int client);
void ignore_remaining_header(rio_t* rio);
void parse_host(char* hostline, char* host, int* port_ptr);
void get_filetype(char* filename, char* filetype);
void build_http_header(char* http_header, char* host, char* filename);
void server_error(int fd, char* casue, char* error_num,
                char* short_message, char* long_message);
void get_host_line(rio_t *client_rio, char *hostline);

#endif