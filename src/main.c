#include "connect.h"
#include "deal.h"

typedef struct sockaddr SA;

int main(int argc, char** argv)
{
	if (argc < 2)
    {
		fprintf(stderr, "usage: %s <port number to bind and listen>\n", argv[0]);
		exit(1);
	}

	int listenfd = Open_listenfd(argv[1]);
	while (True)
	{
		struct sockaddr_storage clientaddr; // 该结构体可以容纳任何大小的套接字，以保持协议无关
		socklen_t clientlen = sizeof(clientaddr);
		int connectfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);

		char hostname[MAXLINE], port[MAXLINE];
		Getnameinfo((SA*)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
		printf("Acecepted connection from <%s, %s>\n", hostname, port);
		deal(connectfd);
		Close(connectfd);
	}

	return 0;
}
