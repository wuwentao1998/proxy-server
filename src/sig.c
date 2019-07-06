#include "sig.h"
#include "wrapper.h"
#include "log.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

// #define DEBUG

void handle_signals()
{
    Signal(SIGCHLD, sigchld_handler);
}

void sigchld_handler()
{
    int pid;
    while ((pid = waitpid(-1, 0, WNOHANG)) > 0)
    {
#ifdef DEBUG
        char log_string[MAXLINE];
        sprintf(log_string,  "Process %d exits.", pid);
        Log(Debug, log_string);
#endif
    }
}
