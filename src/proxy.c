#include "macro.h"
#include "connect.h"
#include "http.h"
#include "wrapper.h"
#include "log.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if (initLog() != 0)
        exit(1);

    if (argc < 2)
    {
	    fprintf(stdout, "usage: %s <port number to bind and listen>\n", argv[0]);
		exit(1);
	}

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

        // handle the request
		deal(clientfd);

		Close(clientfd);
	}

	return 0;
}
