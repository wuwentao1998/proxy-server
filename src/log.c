#include "log.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

char* DirName = "../log";
char* FileName = "../log/proxy.log";

static FILE* FP;

/*
 * EFFECTS: init log file
 * Assign values to static global variables FP
 * ERRORS: -1 for init error
 */
int initLog()
{
    mkdir(DirName, 0777);

    if ((FP = fopen(FileName,"a")) == NULL)
    {
        fprintf(stderr, "fail to open or create log file");
        return -1;
    }

    return 0;
}

/*
 * EFFECTS: write formatted log to file
 */
void Log(enum LOG_TYPE type, const char* msg)
{
    char log_type[MAXWORD];
    switch(type)
    {
        case Error:
        strcpy(log_type, "ERROR");
        break;
        case Debug:
        strcpy(log_type, "DEBUG");
        break;
        default:
        strcpy(log_type, "INFO");
    }

    char time_str[MAXLINE];
    time_t now = time(NULL);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));
    fprintf(FP, "[%s]%s: %s\n", log_type, time_str, msg);
    // Flush after write!!!!
    fflush(FP);
}

