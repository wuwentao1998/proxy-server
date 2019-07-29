#include "log.h"
#include "wrapper.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

static const char* dir_name = "../log";

static pthread_mutex_t* mutex;
static FILE* FP;
static struct tm* cur_time;

/*
 * EFFECTS: init log file
 * Assign value to static global variable FP
 * ERRORS: -1 for init error
 */
int initLog()
{
    mkdir(dir_name, 0777);

    if (setFilePointer() == -1) {
        return -1;
    }

    initMutex();
    return regularCleanLog();
}

/*
 * EFFECTS: periodically clean expired logs
 * ERRORS: -1 for error
 */
int regularCleanLog()
{
    struct itimerval tick;
    memset(&tick, 0, sizeof(tick));
    tick.it_value.tv_sec = 1;
    tick.it_interval.tv_sec = 24 * 60 * 60 * 1000;
    if (setitimer(ITIMER_REAL, &tick, NULL) < 0)
        return -1;

    return 0;
}

/*
 * EFFECTS: init mutex in a shared memory
 * Assign value to static global variable mutex
 */
void initMutex()
{
    mutex = (pthread_mutex_t*) Mmap(NULL, sizeof(pthread_mutex_t),
                                     PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &attr);
}

/*
 * EFFECTS: return filename according to current date
 * Assign value to static global variable cur_time
 */
char* getFileName() {
    time_t now = time(NULL);
    cur_time = localtime(&now);
    char time_str[MAXLINE];
    strftime(time_str, MAXLINE, "%F", cur_time);

    char* file_name = (char*) malloc(sizeof(char) * MAXLINE);
    sprintf(file_name, "%s/%s-proxy.log", dir_name, time_str);
    return file_name;
}

/*
 * EFFECTS: assign value to static global variable FP
 * ERRORS: -1 for init error
 */
int setFilePointer()
{
    char* file_name = getFileName();

    if ((FP = fopen(file_name,"a")) == NULL)
    {
        fprintf(stderr, "fail to open or create log file");
        free(file_name);
        return -1;
    }

    free(file_name);
    return 0;
}

/*
 * EFFECTS: write formatted log to file
 */
void Log(enum LOG_TYPE type, const char* func_name, const char* msg)
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
    struct tm* local = localtime(&now);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", local);

    pthread_mutex_lock(mutex);

    if (local->tm_mday != cur_time->tm_mday)
        setFilePointer();

    fprintf(FP, "[%s], <%s>, %s: %s\n", log_type, func_name, time_str, msg);
    // Flush after write!!!!
    fflush(FP);

    pthread_mutex_unlock(mutex);
}

