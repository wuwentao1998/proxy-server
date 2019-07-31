#include "macro.h"
#include "connect.h"
#include "http.h"
#include "wrapper.h"
#include "log.h"
#include "sig.h"
#include "thread.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct sockaddr SA;

pool job_pool;
int thread_num;

void init()
{
    if (initLog() != 0)
        exit(1);

    // handle signals
    handle_signals();

    pool_init(&job_pool, POOL_SIZE);

    for (int i = 0; i < thread_num; i++)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, worker, NULL);
    }

}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        fprintf(stdout, "usage: %s <port number to bind and listen>\n", argv[0]);
        exit(1);
    }

    thread_num = atoi(argv[2]);

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
		Log(Info, "main", log_string);

        pool_insert(&job_pool, clientfd);
	}

}

void* worker(void* vargp)
{
    pthread_detach(pthread_self());
    while(true)
    {
        int clientfd = pool_fetch(&job_pool);
        if (clientfd >= 0)
        {
            char log_string[MAXLINE];
            sprintf(log_string, "deal with clientfd %d", clientfd);
            Log(Info, "worker", log_string);
            deal(clientfd);
            Close(clientfd);
        }

    }
}



