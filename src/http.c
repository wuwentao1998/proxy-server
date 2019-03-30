#include "http.h"

/*
 * EFFECTS: transfer request from client to end server
 *          then receive message from end server and transfer to client
*/
void deal(int clientfd)
{
    /* read request line and headers */
    char buffer[MAXLINE], method[MAXLINE], URL[MAXLINE], version[MAXLINE];
    rio_t client_rio;

    Rio_readinitb(&client_rio, clientfd);
    Rio_readlineb(&client_rio, buffer, MAXLINE);
    // printf("Request headers:\n");
    // printf("%s", buffer);
    sscanf(buffer, "%s %s %s", method, URL, version);
    if (strcmp(method, "GET") != 0)
    {
        server_error(clientfd, method, "501", "Not implemented",
        "Sorry, this proxy can't implement this method");
        return;
    }

    ignore_remaining_header(&client_rio);

    /* parse URL */
    char host[MAXLINE], filename[MAXLINE], CGI_args[MAXLINE];
    int port;
    parse_URL(URL, host, filename, CGI_args, &port);

    /* build http header for end server */
    char http_header[MAXLINE];
    build_http_header(http_header, host, filename);

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

        Rio_writen(clientfd , message, n);
    }

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
 * EFFECTS: parse the URI into a filename string and a CGI arguments string
 *          return 1 if it's static content, return 0 if it's dynamic content
 * REQUIRES: the default directory of static content is the current directory
 *           the default directory of dynamic content is ./CGI_bin
 *           the default filename is ./index.html
*/
bool parse_URL(char* URL, char* host, char* filename, char* CGI_args, int* port_ptr)
{
    char* begin = URL;
    if (strstr(URL, "http://") != NULL)
        begin = strstr(URL, "http://") + strlen("http://");

    bool isHost = false;
    if (strstr(begin, ":") != NULL)
    {
        char* end = strstr(begin, ":");
        *end = '\0';
        strcpy(host, begin);
        isHost = true;
        end = end + 1;
        sscanf(end, "%d", port_ptr);
    }
    else
        *port_ptr = 80;

    if (strchr(begin, '/') == NULL)
    {
        strcpy(filename, "./index.html");
        if (!isHost)
            strcpy(host, begin);

        return true;
    }
    else
        URL = strchr(begin, '/');

    if (!strstr(URL, "CGI_bin"))
    {
        strcpy(CGI_args, "");
        strcpy(filename, ".");
        strcat(filename, URL);
        if (URL[strlen(URL) -  1] == '/')
            strcat(filename, "index.html");

        return true;
    }
    else
    {
        char* ptr = strchr(URL, '?');
        if (ptr != 0)
        {
            strcpy(CGI_args, ptr + 1);
            *ptr = '\0';
        }
        else
            strcpy(CGI_args, "");

        strcpy(filename, ".");
        strcat(filename, URL);
        return false;
    }
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

/*
 * EFFECTS: build the http header
*/
void build_http_header(char* http_header, char* host, char* filename)
{
    /* request line */
    sprintf(http_header, _request_header_format, filename + 1); // filename以 . 开头，所以这里 +1

    /* remaining header */
    char _host[MAXLINE];
    sprintf(_host, _host_format, host);
    sprintf(http_header, "%s%s%s%s%s",
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

