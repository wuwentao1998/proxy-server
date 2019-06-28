#ifndef CONNECT_H
#define CONNECT_H

int Open_listenfd(char* port);
int open_listenfd(char* port);

int Open_serverfd(char* hostname, char* port);
int open_serverfd(char* hostname, char* port);

#endif