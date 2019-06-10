#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int listen_socket, server_socket;
    struct sockaddr_in listen_addr, server_addr;
    socklen_t server_addr_size = sizeof(server_addr);

    char msg[] = "hello world!";

    listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket == -1)
    {
        printf("socket error");
        return -1;
    }

    char* listen_port = "9090";
    memset(&listen_addr, 0 , sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    listen_addr.sin_port = htons(atoi(listen_port));

    if (bind(listen_socket, (struct sockaddr*) &listen_addr, sizeof(listen_addr)) == -1)
    {
        printf("bind error");
        return -1;
    }

    if (listen(listen_socket, 1024) == -1)
    {
        printf("listen error");
        return -1;
    }

    server_socket = accept(listen_socket, (struct sockaddr *)&server_addr, &server_addr_size);
    if (server_socket == -1)
    {
        printf("accept error");
        return -1;
    }

    write(server_socket, msg, sizeof(msg));
    close(server_socket);
    close(listen_socket);
    return 0;
}