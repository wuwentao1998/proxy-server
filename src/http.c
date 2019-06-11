#include "http.h"

/*
 * EFFECTS: transfer request from client to end server
 *          then receive message from end server and transfer to client
*/
void deal(int clientfd)
{
    /* read request line and headers */
    char buffer[MAXLINE];
    char headline[MAXLINE], hostline[MAXLINE];
    char method[MAXLINE], URI[MAXLINE], version[MAXLINE];
    rio_t client_rio;

    Rio_readinitb(&client_rio, clientfd);
    Rio_readlineb(&client_rio, headline, MAXLINE);
    get_host_line(&client_rio, hostline);

    /* parse head line */
    sscanf(headline, "%s %s %s", method, URI, version);
    if (strcmp(method, "GET") != 0)
    {
        server_error(clientfd, method, "501", "Not implemented",
        "Sorry, this proxy can't implement this method");
        return;
    }

    /* parse host line */
    int port;
    char host[MAXLINE];
    parse_host(hostline, host, &port);

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
 * EFFECTS: parse the hostline to get host and port
*/
void parse_host(char* hostline, char* host, int* port_ptr)
{
    int i = 0;
    while (hostline[i] != ' ')
        i++;

    hostline = hostline + i + 1;
    char *index;
    if ((index = strchr(hostline, ':')) == NULL)
    {
        *port_ptr = 80;
        strcpy(host, hostline);
        host[strlen(host) - 1] = '\0';
    }
    else
    {
        host[strlen(host) - 1] = '\0';
        *port_ptr = atoi(index + 1);
        *index = '\0';
        strcpy(host, hostline);
    }
}

/*
 * EFFECTS: build the http header
*/
void build_http_header(char* http_header, char* host, char* URI)
{
    char  _header[MAXLINE], _host[MAXLINE];
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

void get_host_line(rio_t *client_rio, char *hostline) {
    char buffer[MAXLINE];
    do {
    Rio_readlineb(client_rio, buffer, MAXLINE);
    } while (strstr(buffer, "Host") == NULL);

    strcpy(hostline, buffer);

    ignore_remaining_header(client_rio);
}
