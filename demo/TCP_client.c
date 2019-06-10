#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    struct sockaddr_in server_addr;

    int client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_sock == -1)
        printf("socket error");

    char* server_ip = "127.0.0.1";
    char* server_port = "9090";
    memset(&server_addr, 0 , sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(atoi(server_port));

    if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        printf("connect error");
        return -1;
    }

    char msg[30];
    int str_len = read(client_sock, msg, sizeof(msg) - 1);
    if (str_len == -1)
    {
        printf("read error");
        return -1;
    }

    printf("msg from server:%s \n", msg);

    close(client_sock);
    return 0;
}