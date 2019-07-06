#include "error.h"
#include "macro.h"
#include "log.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/*
 * EFFECTS: send unix-style error to log file
*/
void unix_error(char* message)
{
    char log_string[MAXLINE];
    sprintf(log_string,  "%s: %s", message, strerror(errno));
    Log(Error, log_string);
    return;
}

/*
 * EFFECTS: send getaddrinfo-style error to log file
*/
void gai_error(int code, char *message)
{
    char log_string[MAXLINE];
    sprintf(log_string,  "%s: %s", message, gai_strerror(code));
    Log(Error, log_string);
    return;
}

/*
 * EFFECTS: send error to log file and exit the program
*/
void fatal_error(char* message)
{
    Log(Error, message);
    exit(1);
}

