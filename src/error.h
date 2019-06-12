#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include "macro.h"

void unix_error(char* message);
void gai_error(int code, char* message);


#endif