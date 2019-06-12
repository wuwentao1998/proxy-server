#include "http.h"

typedef struct sockaddr SA;

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "usage: %s <port number to bind and listen>\n", argv[0]);
		exit(1);
	}
 	// Signal(SIGPIPE,SIG_IGN);

	int listenfd = Open_listenfd(argv[1]);
	while (true)
	{
		struct sockaddr_storage clientaddr; // 该结构体可以容纳任何大小的套接字，以保持协议无关(socckaddr无法容纳IPv6)
		socklen_t clientlen = sizeof(clientaddr);
		int clientfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);

		char hostname[MAXLINE], port[MAXLINE];
		Getnameinfo((SA*)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
		printf("Acecepted connection from <%s, %s>\n", hostname, port);
		deal(clientfd);
		Close(clientfd);
	}

	return 0;
}
