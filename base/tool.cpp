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
#include "OperationCode.h"
#include "SysLogManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <sys/time.h>

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

const char* timeString(void)
{
    struct timeval tv;
    struct tm* timeptr = NULL;
    static char result[64];

    gettimeofday(&tv, NULL);
    timeptr = localtime(&(tv.tv_sec));
    sprintf(result, "%d-%02d-%02d_%02d:%02d:%02d.%06lu",
        1900 + timeptr->tm_year,
        timeptr->tm_mon+1,
        timeptr->tm_mday,
        timeptr->tm_hour,
        timeptr->tm_min,
        timeptr->tm_sec,
        tv.tv_usec);
    return result;
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

void instanceLock(const char* arg0)
{
    int lockfd = 0;
    int bytesread = 0;
    char pbuf[32] = {0};
    char lockfile[64] = {0};

    snprintf(lockfile, sizeof(lockfile)-1, "%s.pid", arg0);
    lockfile[sizeof(lockfile)-1] = '\0';

    lockfd = open(lockfile, O_RDWR | O_CREAT, 0644);
    if (-1 == lockfd) {
        printf("Open lock file \"%s\" FAILED\n"
            "Program closed, press any key to continue... \n",
            lockfile);
        exit(1);
    }
    if (-1 == flock(lockfd, LOCK_EX | LOCK_NB) ) {
        bytesread = read(lockfd, pbuf, sizeof(pbuf)-1);
        pbuf[bytesread] = 0;
        printf("Already have a \"arg0(PID = %s)\" running in this directory, lock FAILED\n"
            "program closed, press any key to continue... \n",
            pbuf);
        exit(1);
    }
    if (-1 == ftruncate(lockfd, 0)) {
        printf("Clear lock file \"%s\" FAILED\n"
            "Program closed, press any key to continue... \n",
            lockfile);
        exit(1);
    }
    snprintf(pbuf, sizeof(pbuf), "%d", getpid());
    if ((ssize_t)strlen(pbuf) != write(lockfd, pbuf, strlen(pbuf))) {
        printf("Record PID(%d) to lock file \"%s\" FAILED\n"
            "Program closed, press any key to continue... \n",
            getpid(),
            lockfile);
        exit(1);
    }

    close(lockfd);
    printf("Instance (PID = %d) lock OK\n", getpid());
    return;
}

void instanceUnlock(const char* arg0)
{
    int result = -1;
    int lockfd = 0;
    int bytesread = 0;
    char pbuf[32] = {0};
    char lockfile[64] = {0};

    snprintf(lockfile, sizeof(lockfile)-1, "%s.pid", arg0);
    lockfile[sizeof(lockfile)-1] = '\0';

    lockfd = open(lockfile, O_RDWR, 0644);
    if (-1 == lockfd) {
        printf("Open lock file \"%s\" FAILED\n",
            lockfile);
        result = errno;
        goto ExitError;
    }

    bytesread = read(lockfd, pbuf, sizeof(pbuf)-1);
    pbuf[bytesread] = 0;
    if (getpid() != atoi(pbuf)) {
        printf("This lock file(PID = %s) is not held by me\n",
            pbuf);
        result = -1;
        goto ExitError;
    }

    if (-1 == flock(lockfd, LOCK_UN | LOCK_NB) ) {
        printf("Call flock() to unlock FAILED\n");
        result = errno;
        goto ExitError;
    }
    if (-1 == remove(lockfile)) {
        printf("Remove lock file \"%s\" FAILED\n",
            lockfile);
        result = errno;
        goto ExitError;
    }

    result = 0;
ExitError:
    if (lockfd != 0) {
        close(lockfd);
    }
    if (result != 0) {
        printf("Instance (PID = %d) unlock FAILED(%d)\n",
            getpid(),
            result);
    } else {
        printf("Instance (PID = %d) unlock OK\n",
            getpid());
    }

    return;
}

/*
 * 日志输出函数
 */
void logger(const uint32_t zone,
    const uint32_t gateway,
    const char*    type,     //desciption of operation
    const int      ret,
    const uint32_t user_id,
    const uint32_t role_id,
    const char*    format,
    ...)
{
    int retcode = E_ERROR;
    uint32_t buf_len = 0;
    static char buf[1024] = {0};
    va_list arg;

    va_start(arg, format);
    buf_len = vsprintf(buf, format, arg);
    va_end(arg);

    buf[buf_len] = '\0';

    retcode = SysLogManager::output(
        (SysLog::LOG_TYPE)(SysLog::LOG_INFO),
        "%u|%u|%s|%d|%u|%u|%s\n",
        zone,
        gateway,
        type,
        ret,
        user_id,
        role_id,
        buf);
#ifdef DEBUG
    printf("%s|%u|%u|%s|%d|%u|%u|%s\n",
        timeString(),
        zone,
        gateway,
        type,
        ret,
        user_id,
        role_id,
        buf);
#endif // DEBUG

    return;
}

void logger(const char* format, ...)
{
    int retcode = E_ERROR;
    uint32_t buf_len = 0;
    static char buf[MAX_BUFFER_SIZE] = {0};
    va_list arg;

    va_start(arg, format);
    buf_len = vsprintf(buf, format, arg);
    va_end(arg);

    buf[buf_len] = '\0';

    retcode = SysLogManager::output((SysLog::LOG_TYPE)(SysLog::LOG_INFO), "%s\n", buf);
#ifdef DEBUG
    printf("%s %s\n", timeString(), buf);
#endif // DEBUG
    return;
}

