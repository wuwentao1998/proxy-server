#ifndef DEAL_H
#define DEAL_H
#include <stdbool.h>
#include<sys/stat.h>

#include "wrapper.h"

void deal(int connectfd);
void ignore_header(rio_t* rio);
int parse_URI(char* URI, char* filename, char* CGI_args);
void serve_static(int fd, char* filename, int filesize);
void serve_dynamic(int fd, char* filename, char* CGI_args);
void get_filetype(char* filename, char* filetype);

#endif