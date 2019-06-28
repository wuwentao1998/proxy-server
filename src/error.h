#ifndef ERROR_H
#define ERROR_H

#include "macro.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

void unix_error(char* message);
void gai_error(int code, char* message);


#endif