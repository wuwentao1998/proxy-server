#include "error.h"

/*
 * EFFECTS: send unix-style error to stderr
*/
void unix_error(char* message)
{
    fprintf(stderr, "%s: %s\n", message, strerror(errno));
    exit(1);
}