#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "macro.h"

void unix_error(char* message);
void gai_error(int code, char* message);
void server_error(int fd, char* casue, char* error_num,
                char* short_message, char* long_message);
                
#endif