#ifndef ROBUST_IO_H
#define ROBUST_IO_H

#include "macro.h"
#include "error.h"
#include "wrapper.h"
#include <unistd.h>


// 内置的缓冲区保证了线程安全
typedef struct {
    int rio_fd;                // Descriptor for rio_buf
    int rio_cnt;               // Unread bytes in rio_buf
    char *rio_bufptr;          // Next unread byte in rio_buf
    char rio_buf[RIO_BUFSIZE]; // Internal buffer
} rio_t;

/* ssize_t 是有符号的，而size_t是无符号的。因为出错时返回-1，所以使用ssize_t */
ssize_t Rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_readn(int fd, void *usrbuf, size_t n);

void Rio_writen(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);

void Rio_readinitb(rio_t *rp, int fd);
void rio_readinitb(rio_t *rp, int fd);

// read是系统调用，需要陷入内核态，使用缓冲区可以减少切换
// write没有带缓冲区的函数，否则每次都需要手动刷新缓冲区以写入文件
ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t	rio_readnb(rio_t *rp, void *usrbuf, size_t n);

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t	rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

/* helper function */
ssize_t rio_read(rio_t* rp, char* usrbuf, size_t n);
#endif