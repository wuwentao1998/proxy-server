#ifndef CONNECT_H
#define CONNECT_H
#include <stdio.h>
#include <errno.h>
// #include <netdb.h>

#define True 1
#define False 0

#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define BACKLOG 1024

void unix_error(char* msg);

int Open_listenfd(char* port);
int open_listenfd(char* port);

int Open_clientfd(char* hostname, char* port);
int open_clientfd(char* hostname, char* port);

#endif