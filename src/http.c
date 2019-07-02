#include "http.h"
#include "connect.h"
#include "error.h"
#include "wrapper.h"
#include <stdio.h>
#include<sys/stat.h>
#include <string.h>
#include <stdlib.h>

/*
 * EFFECTS: transfer request from client to end server
 *          then receive message from end server and transfer to client
*/
void deal(int clientfd)
{
    /* read request line and headers */
    char buffer[MAXLINE];
    char headline[MAXLINE];
    char method[MAXWORD], URL[MAXWORD], version[MAXWORD];
    rio_t client_rio;

    Rio_readinitb(&client_rio, clientfd);
    Rio_readlineb(&client_rio, headline, MAXLINE);
    ignore_remaining_header(&client_rio);

    /* parse head line */
    sscanf(headline, "%s %s %s", method, URL, version);
    if (strcmp(method, "GET") != 0)
    {
        server_error(clientfd, method, "501", "Not implemented",
        "Sorry, this proxy can't implement this method.");
        return;
    }

    int port;
    char host[MAXWORD], URI[MAXWORD];
    if (parse_URL(URL, URI, host, &port) != 0) {
        server_error(clientfd, method, "400", "invalid url",
                "Sorry, this url is invalid.");
        return;
    }

    /* build http header for end server */
    char http_header[MAXLINE];
    build_http_header(http_header, host, URI);

    /* connect to end server and send request*/
    char port_string[PORT_LEN];
    sprintf(port_string, "%d", port);
    int end_serverfd = Open_serverfd(host, port_string);
    Rio_writen(end_serverfd, http_header, strlen(http_header));
    shutdown(end_serverfd, SHUT_WR);

    /* receive message from server, send it to client */
    char message[MAX_OBJECT_SIZE];
    int message_size = 0;
    rio_t server_rio;
    Rio_readinitb(&server_rio, end_serverfd);

    size_t n;
    while((n = Rio_readlineb(&server_rio, buffer, MAXLINE))!= 0)
    {
        message_size += n;
        if(message_size < MAX_OBJECT_SIZE)
            strcat(message, buffer);
        else
        {
            Rio_writen(clientfd , message, message_size);
            message_size = 0;
            message[0] = '\0';
        }
    }
    Rio_writen(clientfd , message, message_size);

    Close(end_serverfd);
}

/*
 * EFFECTS: read and ignore the remaining request header
*/
void ignore_remaining_header(rio_t* rio)
{
    char buffer[MAXLINE];

    Rio_readlineb(rio, buffer, MAXLINE);
    while (strcmp(buffer, "\r\n") != 0)
        Rio_readlineb(rio, buffer, MAXLINE);
}

/*
 * EFFECTS: parse the URL to get URI, host and port
 * ERRORS: -1 for incorrect url
*/
int parse_URL(char* URL, char* URI, char* host, int* port_ptr)
{
    // handle protocol prefix
    if (strstr(URL, "http://") != NULL)
        URL = URL + 7;
    else if (strstr(URL, "https://") != NULL)
        URL = URL + 8;
    else if (strstr(URL, "://") != NULL)
        return -1;

    if (strchr(URL, ':') == NULL)
    {
        char* url_begin = strchr(URL, '/');
        if (url_begin == NULL)
            strcpy(URI, "/");
        else
        {
            strcpy(URI, url_begin);
            *url_begin = '\0';
        }

        strcpy(host, URL);
        if (strlen(host) == 0)
            return -1;

        *port_ptr = 80;
    }
    else
    {
        char* host_end = strchr(URL, ':');
        char* url_begin = strchr(URL, '/');
        if (host_end >= url_begin - 1)
            return -1;

        if (url_begin == NULL)
            strcpy(URI, "/");
        else
        {
            strcpy(URI, url_begin);
            *url_begin = '\0';
        }

        *host_end = '\0';
        strcpy(host, URL);
        if (strlen(host) == 0)
            return -1;

        *port_ptr = atoi(host_end + 1);
    }

    return 0;
}

/*
 * EFFECTS: build the http header
*/
void build_http_header(char* http_header, char* host, char* URI)
{
    char  _header[MAXLINE], _host[MAXWORD];
    sprintf(_header, _request_header_format, URI);
    sprintf(_host, _host_format, host);
    sprintf(http_header, "%s%s%s%s%s%s",
            _header,
            _host,
            _connection,
            _proxy,
            _user_agent,
            _blank_line);
}


/*
 * EFFECTS: Report server error to client
*/
void server_error(int fd, char* cause, char* error_num,
                char* short_message, char* long_message)
{
    char buffer[MAXLINE], body[MAXLINE];

    /* build the HTTP response body */
    sprintf(body, "<html><title> Server Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, error_num, short_message);
    sprintf(body, "%s<p>%s: %s \r\n", body, long_message, cause);
    sprintf(body, "%s<hr><em>The Proxy Server</em>\r\n", body);

    /* print the HTTP response */
    sprintf(buffer, "HTTP/1.0 %s %s\r\n", error_num, short_message);
    Rio_writen(fd, buffer, strlen(buffer));
    sprintf(buffer, "Content-type: text/html\r\n");
    Rio_writen(fd, buffer, strlen(buffer));
    sprintf(buffer, "Content-length: %ld\r\n\r\n", strlen(body));
    Rio_writen(fd, buffer, strlen(buffer));
    Rio_writen(fd, body, strlen(body));
}

