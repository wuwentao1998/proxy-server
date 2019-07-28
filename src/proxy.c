#include "macro.h"
#include "connect.h"
#include "http.h"
#include "wrapper.h"
#include "log.h"
#include "sig.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdlib.h>

#define DEBUG

typedef struct sockaddr SA;

void init()
{
    if (initLog() != 0)
        exit(1);

    // handle signals
    handle_signals();
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stdout, "usage: %s <port number to bind and listen>\n", argv[0]);
        exit(1);
    }

    init();

    int listenfd = Open_listenfd(argv[1]);
    if (listenfd < 0)
        exit(1);

    while (true)
	{
        struct sockaddr_storage clientaddr; // 该结构体可以容纳任何大小的套接字，以保持协议无关(socckaddr无法容纳IPv6)
		socklen_t clientlen = sizeof(clientaddr);
		int clientfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);
        if (clientfd < 0)
            continue;

		char hostname[MAXWORD], port[MAXWORD];
		Getnameinfo((SA*)&clientaddr, clientlen, hostname, MAXWORD, port, MAXWORD, 0);

        char log_string[MAXLINE];
        sprintf(log_string, "Acecepted connection from <%s, %s>", hostname, port);
		Log(Info, log_string);

        int pid = Fork();
        if (pid < 0){
            Close(clientfd);
            continue;
        }
        else if (pid == 0)
        {
            Close(listenfd);
            // handle the request
            deal(clientfd);
            Close(clientfd);
            exit(0);
        }

#ifdef DEBUG
        sprintf(log_string, "Process %d starts.", pid);
        Log(Debug, log_string);
#endif

        // Parent should also close clientfd!
		Close(clientfd);
	}

}


