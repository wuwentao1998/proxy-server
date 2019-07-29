#ifndef WRAPPER_H
#define WRAPPER_H

#include "macro.h"
#include <sys/socket.h>

typedef void (*handler_t)(int);


void Close(int fd);
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
void* Mmap(void* addr, size_t len, int port, int flags, int fd, off_t offset);
void Munmap(void* start, size_t len);
void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags);
pid_t Fork();
void Signal(int signum, handler_t handler);

#endif