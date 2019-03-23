#include "../src/http.h"
#include <assert.h>

void test_1()
{
    char host[MAXLINE], filename[MAXLINE], CGI_args[MAXLINE];
    int port;

    char URL[MAXLINE] = "www.google.com";
    bool is_static = parse_URL(URL, host, filename, CGI_args, &port);

    assert(is_static == true);
    assert(port == 80);
    assert(strcmp(host, "www.google.com") == 0);
    assert(strcmp(filename, "./home.html") == 0);
}

void test_2()
{
    char host[MAXLINE], filename[MAXLINE], CGI_args[MAXLINE];
    int port;

    char URL[MAXLINE] = "www.google.com:1024";
    bool is_static = parse_URL(URL, host, filename, CGI_args, &port);

    assert(is_static == true);
    assert(port == 1024);
    assert(strcmp(host, "www.google.com") == 0);
    assert(strcmp(filename, "./home.html") == 0);
}

void test_3()
{
    char host[MAXLINE], filename[MAXLINE], CGI_args[MAXLINE];
    int port;

    char URL[MAXLINE] = "www.google.com/";
    bool is_static = parse_URL(URL, host, filename, CGI_args, &port);

    assert(is_static == true);
    assert(port == 80);
    assert(strcmp(host, "www.google.com") == 0);
    assert(strcmp(filename, "./home.html") == 0);

}

void test_4()
{
    char host[MAXLINE], filename[MAXLINE], CGI_args[MAXLINE];
    int port;

    char URL[MAXLINE] = "www.google.com:1024/";
    bool is_static = parse_URL(URL, host, filename, CGI_args, &port);

    assert(is_static == true);
    assert(port == 1024);
    assert(strcmp(host, "www.google.com") == 0);
    assert(strcmp(filename, "./home.html") == 0);

}

void test_5()
{
    char host[MAXLINE], filename[MAXLINE], CGI_args[MAXLINE];
    int port;

    char URL[MAXLINE] = "http://www.google.com:1024/";
    bool is_static = parse_URL(URL, host, filename, CGI_args, &port);

    assert(is_static == true);
    assert(port == 1024);
    assert(strcmp(host, "www.google.com") == 0);
    assert(strcmp(filename, "./home.html") == 0);

}

void test_6()
{
    char host[MAXLINE], filename[MAXLINE], CGI_args[MAXLINE];
    int port;

    char URL[MAXLINE] = "http://www.google.com:1024";
    bool is_static = parse_URL(URL, host, filename, CGI_args, &port);

    assert(is_static == true);
    assert(port == 1024);
    assert(strcmp(host, "www.google.com") == 0);
    assert(strcmp(filename, "./home.html") == 0);

}

void test_7()
{
    char host[MAXLINE], filename[MAXLINE], CGI_args[MAXLINE];
    int port;

    char URL[MAXLINE] = "http://www.google.com";
    bool is_static = parse_URL(URL, host, filename, CGI_args, &port);

    assert(is_static == true);
    assert(port == 80);
    assert(strcmp(host, "www.google.com") == 0);
    assert(strcmp(filename, "./home.html") == 0);

}

void test_8()
{
    char host[MAXLINE], filename[MAXLINE], CGI_args[MAXLINE];
    int port;

    char URL[MAXLINE] = "http://www.google.com/";
    bool is_static = parse_URL(URL, host, filename, CGI_args, &port);

    assert(is_static == true);
    assert(port == 80);
    assert(strcmp(host, "www.google.com") == 0);
    assert(strcmp(filename, "./home.html") == 0);

}

void test_9()
{
    char host[MAXLINE], filename[MAXLINE], CGI_args[MAXLINE];
    int port;

    char URL[MAXLINE] = "http://www.google.com/home.txt";
    bool is_static = parse_URL(URL, host, filename, CGI_args, &port);

    assert(is_static == true);
    assert(port == 80);
    assert(strcmp(host, "www.google.com") == 0);
    assert(strcmp(filename, "./home.txt") == 0);
}

void test_10()
{
    char host[MAXLINE], filename[MAXLINE], CGI_args[MAXLINE];
    int port;

    char URL[MAXLINE] = "http://www.google.com/CGI_bin/add?123&456";
    bool is_static = parse_URL(URL, host, filename, CGI_args, &port);

    assert(is_static == false);
    assert(port == 80);
    assert(strcmp(host, "www.google.com") == 0);
    assert(strcmp(filename, "./CGI_bin/add") == 0);
    assert(strcmp(CGI_args, "123&456") == 0);
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
    test_10();
}