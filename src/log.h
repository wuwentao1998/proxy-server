#ifndef LOG_H
#define LOG_H

#include "macro.h"

int initLog();
void Log(enum LOG_TYPE type, const char* msg);

#endif //LOG_H

