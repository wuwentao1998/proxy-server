#include "http.h"

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

/*
 * EFFECTS: parse the URI into a filename string and a CGI arguments string
 *          return 1 if it's static content, return 0 if it's dynamic content
 * REQUIRES: the default directory of static content is the current directory
 *           the default directory of dynamic content is ./CGI-bin
 *           the default filename is ./home.html
*/
int parse_URI(char* URI, char* filename, char* CGI_args)
{
    if (!strstr(URI, "CGI-bin"))
    {
        strcpy(CGI_args, "");
        strcpy(filename, ".");
        strcat(filename, URI);
        if (URI[strlen(URI) -  1] == '/')
            strcat(filename, "home.html");

        return 1;
    }
    else
    {
        char* ptr = strchr(URI, '?');
        if (ptr != 0)
        {
            strcpy(CGI_args, ptr + 1);
            *ptr = '\0';
        }
        else
            strcpy(CGI_args, "");

        strcpy(filename, ".");
        strcat(filename, URI);
        return 0;
    }
}

/*
 * EFFECTS: process and provide static content
 *          types of files that can be processed: text/html, text/plain, image/gif and image/png
*/
void serve_static(int fd, char* filename, int filesize)
{
    /* send response header */
    char filetype[MAXLINE], buffer[MAXLINE];
    get_filetype(filename, filetype);
    sprintf(buffer, "HTTP/1.0 200 OK\r\n");
    sprintf(buffer, "%sConnection: close\r\n", buffer);
    sprintf(buffer, "%sContent-length: %d \r\n", buffer, filesize);
    sprintf(buffer, "%sContent-type: %s\r\n", buffer, filetype);
    Rio_writen(fd, buffer, strlen(buffer));
    printf("Response headers:\n");
    printf("%s", buffer);

    /* send response body */
    int src_fd = Open(filename, O_RDONLY, 0);
    char* src_ptr = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, src_fd, 0);
    Close(src_fd);
    Rio_writen(fd, src_ptr, filesize);
    Munmap(src_ptr, filesize);
}

/*
 * EFFECTS: process and provide static content
 *          fork a subprocess and run the CGI program
*/
void serve_dynamic(int fd, char* filename, char* CGI_args)
{
    char buffer[MAXLINE];

    sprintf(buffer, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buffer, strlen(buffer));

    char* emptylist[] = {NULL};
    /* child */
    if (Fork() == 0)
    {
        setenv("QUERY_STRING", CGI_args, 1);
        Dup2(fd, STDOUT_FILENO);
        Execve(filename, emptylist, environ);
    }

    /* parent */
    Wait(NULL);
}

/*
 * EFFECTS: derive file type from filename
*/
void get_filetype(char* filename, char* filetype)
{
    if (strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
    else if (strstr(filename, ".png"))
        strcpy(filetype, "image/png");
    else
        strcpy(filetype, "text/plain");
}