#include "error.h"

/*
 * EFFECTS: send unix-style error to stderr
*/
void unix_error(char* message)
{
    fprintf(stderr, "%s: %s\n", message, strerror(errno));
    exit(1);
}

/*
 * EFFECTS: send getaddrinfo-style error to stderr
*/
void gai_error(int code, char *message)
{
    fprintf(stderr, "%s: %s\n", message, gai_strerror(code));
    exit(1);
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
    sprintf(buffer, "Content-length: %d\r\n\r\n", strlen(body));
    Rio_writen(fd, buffer, strlen(buffer));
    Rio_writen(fd, body, strlen(body));
}