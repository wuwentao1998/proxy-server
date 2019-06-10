#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10

int main(int argc, char* argv[])
{
    int server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket == -1)
    {
        printf("socket error");
        return -1;
    }

    char* server_port = "8888";
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(server_port));

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        printf("bind error");
        return -1;
    }

    while(true)
    {
        char msg[MAX];
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        // recvfrom并不会填充client_addr_size，还是需要实现sizeof
        int str_len = recvfrom(server_socket, msg, MAX, 0, (struct sockaddr*)&client_addr, &client_addr_size);
        if (str_len == -1)
        {
            printf("receive error");
            return -1;
        }

        printf("client address: %s,client port: %d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);

        printf("message from client: %s \n", msg);

        str_len = sendto(server_socket, msg, str_len, 0, (struct sockaddr*)&client_addr, client_addr_size);
        if (str_len == -1)
        {
            printf("send error");
            return -1;
        }
    }

    close(server_socket);
    return 0;
}