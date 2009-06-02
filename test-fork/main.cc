#include "tool.h"
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <syslog.h>
#include <inttypes.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int retcode = -1;
    int child_count = 0;
    time_t sec = 0;
    pid_t child_pid = -1;

    daemon(1, 1);
    openlog("###", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_ERR | LOG_USER, "############ %d\n", 1234567);

    while (child_count++ < 5) {
        if ((child_pid = vfork()) < 0) {
            printf("create child process failed\n");
            perror("fork");
            continue;
        }
        if (0 == child_pid) {
            printf("child process %u oops\n", getpid());
            if ((retcode = execlp("./sleep30", NULL)) < 0) {
                perror("execlp");
                _exit(0);
            }
        } else {
            printf("create child OK, PID(%u)\n", child_pid);
        }
    }

    while ((child_pid = waitpid(-1, NULL, WNOHANG)) >= 0) {
        if (child_pid > 0) {
            printf("child process %u exit\n", child_pid);
        }
        syslog(LOG_ERR | LOG_USER, "passed seconds = %u\n", (uint32_t)sec++);
        sleep(1);
    }
    PUT_ERR("parent waitpid error");

    sleep(3);
    printf("parent wake up\n");
    printf("parent exit OK\n");

    return 0;
}

