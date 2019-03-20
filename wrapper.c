#include "wrapper.h"

/*
 * EFFECTS: wrapper funtion for close
 *          add error handling
*/
void Close(int fd)
{
    int error = close(fd);

    if (error < 0)
	    unix_error("Close error");
}

/*
 * EFFECTS: wrapper funtion for accept
 *          add error handling
*/
int Accept(int listenfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int fd = accept(listenfd, addr, addrlen);

    if (fd < 0)
	    unix_error("Accept error");

    return fd;
}

/*
 * EFFECTS: wrapper funtion for getnameinfo
 *          add error handling
*/
void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags)
{
    int error = getnameinfo(sa, salen, host, hostlen, serv,
                          servlen, flags);

    if (error != 0)
        gai_error(error, "Getnameinfo error");
}