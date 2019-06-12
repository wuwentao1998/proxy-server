#ifndef CONNECT_H
#define CONNECT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "error.h"
#include "macro.h"

int Open_listenfd(char* port);
int open_listenfd(char* port);

int Open_serverfd(char* hostname, char* port);
int open_serverfd(char* hostname, char* port);

#endif