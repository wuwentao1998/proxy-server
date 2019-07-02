#include "../src/http.h"
#include <assert.h>
#include <string.h>

void test_1()
{
    char host[MAXLINE];
    char URI[MAXLINE];
    int port;

    char URL[MAXLINE] = "www.baidu.com/index.html";
    parse_URL(URL, URI, host, &port);

    assert(port == 80);
    assert(strcmp(host, "www.baidu.com") == 0);
    assert(strcmp(URI, "/index.html") == 0);
}

void test_2()
{
    char host[MAXLINE];
    char URI[MAXLINE];
    int port;

    char URL[MAXLINE] = "http://www.baidu.com/index.html";
    parse_URL(URL, URI, host, &port);

    assert(port == 80);
    assert(strcmp(host, "www.baidu.com") == 0);
    assert(strcmp(URI, "/index.html") == 0);
}

void test_3()
{
    char host[MAXLINE];
    char URI[MAXLINE];
    int port;

    char URL[MAXLINE] = "https://www.baidu.com/index.html";
    parse_URL(URL, URI, host, &port);

    assert(port == 80);
    assert(strcmp(host, "www.baidu.com") == 0);
    assert(strcmp(URI, "/index.html") == 0);
}

void test_4()
{
    char host[MAXLINE];
    char URI[MAXLINE];
    int port;

    char URL[MAXLINE] = "https://www.baidu.com:8080/dir1/dir2/main";
    parse_URL(URL, URI, host, &port);

    assert(port == 8080);
    assert(strcmp(host, "www.baidu.com") == 0);
    assert(strcmp(URI, "/dir1/dir2/main") == 0);
}

void test_5()
{
    char host[MAXLINE];
    char URI[MAXLINE];
    int port;

    char URL[MAXLINE] = "https://www.baidu.com:8080";
    parse_URL(URL, URI, host, &port);

    assert(port == 8080);
    assert(strcmp(host, "www.baidu.com") == 0);
    assert(strcmp(URI, "/") == 0);
}

void test_6()
{
    char host[MAXLINE];
    char URI[MAXLINE];
    int port;

    char URL[MAXLINE] = "www.baidu.com";
    parse_URL(URL, URI, host, &port);

    assert(port == 80);
    assert(strcmp(host, "www.baidu.com") == 0);
    assert(strcmp(URI, "/") == 0);
}

void test_7()
{
    char host[MAXLINE];
    char URI[MAXLINE];
    int port;

    char URL[MAXLINE] = "www.baidu.com/";
    parse_URL(URL, URI, host, &port);

    assert(port == 80);
    assert(strcmp(host, "www.baidu.com") == 0);
    assert(strcmp(URI, "/") == 0);
}

void test_8()
{
    char host[MAXLINE];
    char URI[MAXLINE];
    int port;

    char URL[MAXLINE] = "ftp://www.baidu.com/";
    int err = parse_URL(URL, URI, host, &port);

    assert(err != 0);
}

void test_9()
{
    char host[MAXLINE];
    char URI[MAXLINE];
    int port;

    char URL[MAXLINE] = "http://www.baidu.com:/";
    int err = parse_URL(URL, URI, host, &port);

    assert(err != 0);
}

int main(void)
{
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();
    test_7();
    test_8();
    test_9();
}