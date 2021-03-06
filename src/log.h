#ifndef LOG_H
#define LOG_H

#include "macro.h"

int initLog();
void initMutex();
char* getFileName();
int setFilePointer();
int regularCleanLog();
void Log(enum LOG_TYPE type, const char* func_name, const char* msg);

#endif //LOG_H

