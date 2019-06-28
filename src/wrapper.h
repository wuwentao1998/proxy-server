#ifndef WRAPPER_H
#define WRAPPER_H

#include <sys/socket.h>

/* External variables */
// extern char **environ;

/* Unix I/O wrappers */
void Close(int fd);
int Open(const char* pathname, int flags, mode_t mode);
int Dup2(int fd1, int fd2);

/* Socket interface wrappers */
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);

/* Memory mapping wrappers */
void* Mmap(void* addr, size_t len, int port, int flags, int fd, off_t offset);
void Munmap(void* start, size_t len);

/* Protocol independent wrappers */
void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags);

/* Process control wrappers */
pid_t Fork();
void Execve(const char *filename, char *const argv[], char *const envp[]);
pid_t Wait(int *status);


#endif