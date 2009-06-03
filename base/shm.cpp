#include "shm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int32_t shmCreate(key_t key, size_t size, int32_t shmflg)
{
    int32_t shmid = -1;
    if ((shmid = shmget(key, size, shmflg)) < 0) {
        printf("[key=0x%08X] ", (int32_t)key);
        PUT_ERR("shmCreate");
    }
    return shmid;
}

int32_t shmCreate(uint8_t appID, size_t size, int32_t shmflg)
{
    key_t key = ftok(getenv("PWD"), appID);
    return shmCreate(key, size, shmflg);
}

int32_t shmGet(key_t key, int32_t shmflg)
{
    int32_t shmid = -1;
    if ((shmid = shmget(key, 0, shmflg)) < 0) {
        PUT_ERR("shmGet");
    }
    return shmid;
}

int32_t shmGet(uint8_t appID, int32_t shmflg)
{
    key_t key = ftok(getenv("PWD"), appID);
    return shmGet(key, shmflg);
}

void* shmAt(int32_t shmid, const void* shmaddr, int32_t shmflg)
{
    void* addr = NULL;
    if ((uint32_t)(addr = shmat(shmid, shmaddr, shmflg)) < 0) {
        PUT_ERR("shmAt");
    }
    return addr;
}

int32_t shmDt(const void* shmaddr)
{
    int32_t retcode = -1;
    if ((retcode = shmdt(shmaddr)) < 0) {
        PUT_ERR("shmDt");
    }
    return retcode;
}

int32_t shmDestroy(int32_t shmid)
{
    int32_t retcode = -1;
    if ((retcode = shmctl(shmid, IPC_RMID, NULL)) < 0) {
        PUT_ERR("shmDestroy");
    }
    return retcode;
}

