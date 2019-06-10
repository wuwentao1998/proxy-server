#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 20

int main(int argc, char* argv[])
{
    int client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_socket == -1)
    {
        printf("socket error");
        return -1;
    }

    char* address = "127.0.0.1";
    char* port = "8888";
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(address);
    client_addr.sin_port = htons(atoi(port));

    char msg[MAX];
    while(true)
    {
        fputs("input message(q to quit\n", stdout);
        fgets(msg, MAX, stdin);
        if (!strcmp(msg, "q\n"))
            break;
    
        int str_len = sendto(client_socket, msg, strlen(msg), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
        if (str_len == -1)
        {
            printf("send error");
            return -1;
        }
        struct sockaddr_in server_addr;
        socklen_t addr_size;
        str_len = recvfrom(client_socket, msg, MAX, 0, (struct sockaddr*)&server_addr, &addr_size);
        if (str_len == -1)
        {
            printf("receive error");
            return -1;
        }

        msg[str_len] = '\0';
        printf("message from server: %s \n", msg);
    }

    close(client_socket);
    return 0;
}