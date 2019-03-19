#ifndef WRAPPER_H
#define WRAPPER_H

#include "error.h"

void Close(int fd);
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags);


#endif