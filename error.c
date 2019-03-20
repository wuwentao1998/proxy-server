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