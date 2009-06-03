#ifndef _SHM_H_
#define _SHM_H_

#include "tool.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
/*{{{*/
/*
class SHM {
public:
    SHM(void);
    ~SHM(void);

    int32_t initialize(uint32_t appID);
    int32_t release(void);

private:
    SHM(const SHM& rhs);
    SHM& operator = (const SHM& rhs);
}; */ /*}}}*/

const int32_t DEF_PERM = 0644;

int32_t shmCreate(key_t key, size_t size, int32_t shmflg = IPC_CREAT | DEF_PERM);
int32_t shmCreate(uint8_t appID, size_t size, int32_t shmflg = IPC_CREAT | DEF_PERM);

int32_t shmGet(key_t key, int32_t shmflg = DEF_PERM);
int32_t shmGet(uint8_t appID, int32_t shmflg = DEF_PERM);

void* shmAt(int32_t shmid, const void* shmaddr = NULL, int32_t shmflg = 0);
int32_t shmDt(const void* shmaddr);

int32_t shmDestroy(int32_t shmid);

#endif //  _SHM_H_

