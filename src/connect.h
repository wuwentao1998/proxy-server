#ifndef CONNECT_H
#define CONNECT_H

#include <netdb.h>
#include "macro.h"
#include "error.h"

int Open_listenfd(char* port);
int open_listenfd(char* port);

int Open_clientfd(char* hostname, char* port);
int open_clientfd(char* hostname, char* port);

#endif