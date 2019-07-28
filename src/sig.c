#include "sig.h"
#include "wrapper.h"
#include "log.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


// #define DEBUG
static char* shell_path = "../log/clean.sh";

void handle_signals()
{
    Signal(SIGCHLD, sigchld_handler);
    Signal(SIGALRM, sigalrm_hander);
}

void sigchld_handler()
{
    sigset_t mask, prev_mask;
    sigfillset(&mask);

    int pid, status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        // 避免信号处理程序被中断
        sigprocmask(SIG_BLOCK, &mask, &prev_mask);
        // 判断子进程是否异常退出
        if (WIFEXITED(status) == 0)
        {
            char log_string[MAXLINE];
            sprintf(log_string,  "Process %d exits abnormally.", pid);
            Log(Error, log_string);
        }
#ifdef DEBUG
        char log_string[MAXLINE];
        sprintf(log_string,  "Process %d exits.", pid);
        Log(Debug, log_string);
#endif

        sigprocmask(SIG_SETMASK, &prev_mask, NULL);
    }

}

void sigalrm_hander()
{
    sigset_t mask, prev_mask;
    sigfillset(&mask);

    // 避免信号处理程序被中断
    sigprocmask(SIG_BLOCK, &mask, &prev_mask);
    // 调用shell脚本
    system(shell_path);
    sigprocmask(SIG_SETMASK, &prev_mask, NULL);
}
