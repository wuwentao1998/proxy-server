#include "connect.h"
#include "error.h"
#include "macro.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/*
 * EFFECTS: wrapper funtion for open_listenfd
 *          add error handling
*/

int Open_listenfd(char* port)
{
    int fd = open_listenfd(port);
    if (fd < 0)
        unix_error("Open_listenfd error");

    return fd;
}

/*
 * EFFECTS: open and return a listening socket on port
 * ERRORS: -1 for getaddrinfo error; -2 for other errors
 */
int open_listenfd(char* port)
{
    struct addrinfo hints;
    struct addrinfo* list_ptr;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;

    int error_code = getaddrinfo(NULL, port, &hints, &list_ptr);
    if (error_code != 0)
    {
        fprintf(stderr, "Getaddrinfo failed (with port number: %s) : %s\n",
         port, gai_strerror(error_code));
        return -1;
    }

    int listenfd = -1;
    struct addrinfo* p = NULL;
    for (p = list_ptr; p != NULL; p = p->ai_next)
    {
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;

        // 服务器默认在重启后30S内拒绝客户端的链接，这里我们改为重启后立即开始接受请求
        int optval = 1;
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break;
        else if (close(listenfd) < 0)
        {
            fprintf(stderr, "Can't close listenfd: %s\n", strerror(errno));
            return - 2;
        }
    }

    freeaddrinfo(list_ptr);
    if (p == NULL)
        return -2;
    else if (listen(listenfd, BACKLOG) < 0)
    {
        if (close(listenfd) < 0)
            fprintf(stderr, "Can't close listenfd: %s\n", strerror(errno));

        return -2;
    }
    else
        return listenfd;
}

/*
 * EFFECTS: wrapper funtion for open_serverfd
 *          add error handling
*/

int Open_serverfd(char* hostname, char* port)
{
    int fd = open_serverfd(hostname, port);
    if (fd < 0)
        unix_error("Open_serverfd error");

    return fd;
}

/*
 * EFFECTS: open connection to server at <hostname, port> and
 *          return a server file descriptor for client to read and write
 * ERRORS: -1 for getaddrinfo error; -2 for other errors
 */
int open_serverfd(char* hostname, char* port)
{
    struct addrinfo hints;
    struct addrinfo* list_ptr;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV | AI_ADDRCONFIG;

    int error_code = getaddrinfo(hostname, port, &hints, &list_ptr);
    if (error_code != 0)
    {
        fprintf(stderr, "Getaddrinfo failed (with hostname: %s, port number: %s) : %s\n",
         hostname, port, gai_strerror(error_code));
        return -1;
    }

    int serverfd = -1;
    struct addrinfo* p = NULL;
    for (p = list_ptr; p != NULL; p = p->ai_next)
    {
        if ((serverfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;
        else if (connect(serverfd, p->ai_addr, p->ai_addrlen) != -1)
            break;
        else if (close(serverfd) < 0)
        {
            fprintf(stderr, "Can't close serverfd: %s\n", strerror(errno));
            return - 2;
        }
    }

    freeaddrinfo(list_ptr);
    return (p == NULL) ? -2 : serverfd;
}