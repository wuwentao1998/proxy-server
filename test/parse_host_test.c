#include "../src/http.h"
#include <assert.h>

void test_1()
{
    char host[MAXLINE];
    int port;

    char host_line[MAXLINE] = "Host: www.baidu.com\n";
    parse_host(host_line, host, &port);

    assert(port == 80);
    assert(strcmp(host, "www.baidu.com") == 0);
}

void test_2()
{
    char host[MAXLINE];
    int port;

    char host_line[MAXLINE] = "Host: 127.0.0.1:8888\n";
    parse_host(host_line, host, &port);

    assert(port == 8888);
    assert(strcmp(host, "127.0.0.1") == 0);
}


int main(void)
{
    test_1();
    test_2();
}