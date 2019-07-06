#ifndef MACRO_H
#define MACRO_H

#define MAXLINE 2048
#define MAXWORD 256
#define BACKLOG 1024
#define RIO_BUFSIZE 4096
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define PORT_LEN 10

typedef struct sockaddr SA;
typedef void (*handler_t)(int);

enum LOG_TYPE
{
    Info, Error, Debug
};

#endif