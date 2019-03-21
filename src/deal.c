#include "deal.h"

/*
 * EFFECTS: handle the HTTP request
 *          read and parse the request
 *          then serve the static or dynamic conetent
*/
void deal(int connectfd)
{
    /* read request line and headers */
    char buffer[MAXLINE], method[MAXLINE], URI[MAXLINE], version[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connectfd);
    Rio_readlineb(&rio, buffer, MAXLINE);
    printf("Request headers:\n");
    printf("%s", buffer);
    sscanf(buffer, "%s %s %s", method, URI, version);
    if (strcmp(method, "GET") != 0)
    {
        client_error(connectfd, method, "501", "Not implemneted",
        "Sorry, this proxy can't implement this method");
        return;
    }

    ignore_header(&rio);

    /* parse URI from the get request */
    char filename[MAXLINE], CGI_args[MAXLINE];
    bool is_static = parse_uri(URI, filename, CGI_args);

    struct stat sbuf;
    if (stat(filename, &sbuf) < 0)
    {
        server_error(connectfd, filename, "404", "Not found",
        "Sorry, proxy can't find this file");
        return;
    }

    /* serve the content */
    if (is_static)
    {
        if (!S_ISREG(subf.st_mode) || !(S_IRUSR & subf.st_mode))
        {
            server_error(connectfd, filename, "403", "Forbidden",
            "Sorry, you have no permission to read this file");
            return;
        }

        serve_static(connectfd, filename, subf.st_size);
    }
    else
    {
        if (!S_ISREG(subf.st_mode) || !(S_IRUSR & subf.st_mode))
        {
            server_error(connectfd, filename, "403", "Forbidden",
            "Sorry, you have no permission to run this CGI program");
            return;
        }

        serve_dynamic(connectfd, filename, CGI_args);
    }
}

/*
 * EFFECTS: read and ignore the request header
*/
void ignore_header(rio_t* rio)
{
    char buffer[MAXLINE];

    Rio_readlineb(rio, buffer, MAXLINE);
    while (strcmp(buffer, "\r\n") != 0)
        Rio_readlineb(rio, buffer, MAXLINE);
}