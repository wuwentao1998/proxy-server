#include "wrapper.h"
#include "error.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>

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
 * EFFECTS: wrapper funtion for mmap
 *          add error handling
*/
void* Mmap(void* addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    void* ptr = mmap(addr, len, prot, flags, fd, offset);

    if (ptr == (void*)(-1))
        unix_error("Mmap error");

    return ptr;
}

/*
 * EFFECTS: wrapper funtion for munmap
 *          add error handling
*/
void Munmap(void* start, size_t len)
{
    if (munmap(start, len) < 0)
        unix_error("Munmap error");
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

/*
 * EFFECTS: wrapper funtion for fork
 *          add error handling
*/
pid_t Fork()
{
    pid_t pid = fork();
    if (pid < 0)
        unix_error("Fork_error");

    return pid;
}


/*
 * EFFECTS: wrapper funtion for signal
 * Errors: exit the program when encountering error
*/
void Signal(int signum, handler_t handler)
{
    handler_t error = signal(signum, handler);
    if (error == SIG_ERR)
        fatal_error("Signal error");
}

