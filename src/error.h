#ifndef ERROR_H
#define ERROR_H

void unix_error(char* message);
void gai_error(int code, char* message);
void fatal_error(char* message);

#endif