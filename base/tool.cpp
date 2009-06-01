// =====================================================================================
// 
//       Filename:  tool.cpp
// 
//    Description:  一些常用的工具类函数
// 
//        Version:  1.0
//        Created:  05/28/09 16:40:20
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Will King (xfun), xfun68@foxmail.com
//        Company:  
// 
// =====================================================================================

#include "tool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/resource.h>

int32_t getMaxFDSize(void)
{
    struct rlimit rt;

    rt.rlim_max = rt.rlim_cur = 0;
    if (getrlimit(RLIMIT_NOFILE, &rt) == -1) {
        return -1;
    } else {
        return (int32_t)rt.rlim_cur;
    }
}

int32_t setMaxFDSize(int32_t size)
{
    struct rlimit rt;
    rt.rlim_max = rt.rlim_cur = size;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1) {
        return -1;
    } else {
        return 0;
    }
}

int32_t runInThread(void* (*threadFunc)(void*), void* args)
{
    int32_t result = -1;
    int32_t retcode = -1;
    pthread_t tid = 0;

    if (0 != (retcode = pthread_create(&tid, NULL,
                threadFunc, args))) {
        result = retcode;
        goto ExitError;
    }

    result = 0;
ExitError:
    return result;
}

const char* time2Str(time_t tm)
{
    struct tm* timeptr = localtime(&tm);
    static char result[64];
    sprintf(result, "%d-%02d-%02d_%02d:%02d:%02d",
        1900 + timeptr->tm_year,
        timeptr->tm_mon+1,
        timeptr->tm_mday,
        timeptr->tm_hour,
        timeptr->tm_min,
        timeptr->tm_sec);
    return result;
}

void instanceRestrict(const char* arg0)
{
    int lockfd = 0;
    int bytesread = 0;
    char pbuf[32] = {0};
    char lockfile[64] = {0};

    snprintf(lockfile, sizeof(lockfile)-1, "%s.pid", arg0);
    lockfile[sizeof(lockfile)-1] = '\0';

    lockfd = open(lockfile, O_RDWR | O_CREAT, 0644);
    if (-1 == lockfd) {
        printf("Open file \"%s\" failed, program closed!\n"
            "Press any key to continue ... \n",
            lockfile);
        exit(1);
    }
    if (-1 == flock(lockfd, LOCK_EX | LOCK_NB) ) {
        bytesread = read(lockfd, pbuf, sizeof(pbuf)-1);
        pbuf[bytesread] = 0;
        printf("Already have a eChargeServer(PID = %s) "
            "running in this directory, program closed!\n"
            "Press any key to continue ... \n",
            pbuf);
        exit(1);
    }
    if (-1 == ftruncate(lockfd, 0)) {
        printf("Clear file \"%s\" failed, program closed!\n"
            "Press any key to continue ... \n",
            lockfile);
        exit(1);
    }
    snprintf(pbuf, sizeof(pbuf), "%d", getpid());
    if ((ssize_t)strlen(pbuf) != write(lockfd, pbuf, strlen(pbuf))) {
        printf("Record PID(%d) to file \"%s\" failed, program closed!\n"
            "Press any key to continue ... \n",
            getpid(), lockfile);
        exit(1);
    }

    close(lockfd);
    printf("Instance Restrict OK! (PID = %d)\n", getpid());
    return;
}

