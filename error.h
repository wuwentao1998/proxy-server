#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <errno.h>

void unix_error(char* message);
void gai_error(int code, char *message);

#endif