#include "shm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int32_t shmCreate(key_t key, size_t size, int32_t shmflg)
{
    int32_t shmid = -1;
    const int32_t MAX_CMD_LEN = 64;
    char cmd[MAX_CMD_LEN+1] = {'\0'};

    if (key < 0xff) {
        key = ftok(getenv("PWD"), key);
    }
    snprintf(cmd, MAX_CMD_LEN, "ipcrm -M 0x%08x >& /dev/null", key);
    system(cmd);
    if ((shmid = shmget(key, size, shmflg)) < 0) {
#ifdef DEBUG_TRACE
        PUT_ERR("#[shmCreate");
#endif //  DEBUG_TRACE
    }
    return shmid;
}

int32_t shmGet(key_t key, int32_t shmflg)
{
    int32_t shmid = -1;
    if (key < 0xff) {
        key = ftok(getenv("PWD"), key);
    }
    if ((shmid = shmget(key, 0, shmflg)) < 0) {
#ifdef DEBUG_TRACE
        PUT_ERR("#[shmGet");
#endif //  DEBUG_TRACE
    }
    return shmid;
}

void* shmAt(int32_t shmid, const void* shmaddr, int32_t shmflg)
{
    void* addr = NULL;
    if ((uint32_t)(addr = shmat(shmid, shmaddr, shmflg)) < 0) {
#ifdef DEBUG_TRACE
        PUT_ERR("#[shmAt");
#endif //  DEBUG_TRACE
    }
    return addr;
}

int32_t shmDt(const void* shmaddr)
{
    int32_t retcode = -1;
    if ((retcode = shmdt(shmaddr)) < 0) {
#ifdef DEBUG_TRACE
        PUT_ERR("#[shmDt");
#endif //  DEBUG_TRACE
    }
    return retcode;
}

int32_t shmDestroy(int32_t shmid)
{
    int32_t retcode = -1;
    if ((retcode = shmctl(shmid, IPC_RMID, NULL)) < 0) {
#ifdef DEBUG_TRACE
        PUT_ERR("#[shmDestroy");
#endif //  DEBUG_TRACE
    }
    return retcode;
}

