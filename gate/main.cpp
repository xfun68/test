#include "gate.h"
#include <stdio.h>
#include <signal.h>
#include <strings.h>

int32_t g_stop = 0;
void sigExit(int signo);

int main(int argc, char *argv[])
{
    int32_t result = -1;
    int32_t retcode = -1;
    Gate gate;

    instanceRestrict(argv[0]);
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, sigExit);
    signal(SIGINT, sigExit);
    signal(SIGTERM, sigExit);

    if (S_SUCCESS != (retcode = gate.initialize())) {
        printf("eRatingGate initialize failed with error [%d]\n",
            retcode);
        goto ExitError;
    }
    printf("eRatingGate initialize OK\n");
    printf("eRatingGate is running ...\n");
    if (S_SUCCESS != (retcode = gate.run())) {
        printf("eRatingGate run failed with error [%d]\n",
            retcode);
        goto ExitError;
    }
    printf("eRatingGate stop OK\n");

    result = 0;
ExitError:
    if ((retcode = gate.release()) < 0) {
        printf("eRatingGate release failed with error [%d]\n",
            retcode);
    }
    printf("eRatingGate release OK\n");

    if (0 == result) {
        puts("program exit OK");
    } else {
        printf("program exit with error[%d]", result);
    }
    return result;
}				// ----------  end of function main  ----------

void sigExit(int signo) {
    g_stop = 1;
    return;
}


