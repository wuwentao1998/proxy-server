#include "log.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>

static char* DirName = "../log";
static char* FileName = "../log/proxy.log";
static pthread_mutex_t* mutex;
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

    initMutex();
    return 0;
}

/*
 * EFFECTS: init mutex in a shared memory
 * Assign values to static global variables mutex
 * ERRORS: -1 for init error
 */
void initMutex()
{
    mutex = (pthread_mutex_t*) mmap(NULL, sizeof(pthread_mutex_t),
                                     PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &attr);
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

    pthread_mutex_lock(mutex);
    fprintf(FP, "[%s]%s: %s\n", log_type, time_str, msg);
    // Flush after write!!!!
    fflush(FP);
    pthread_mutex_unlock(mutex);
}

