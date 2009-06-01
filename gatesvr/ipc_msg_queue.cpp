// =====================================================================================/*{{{*/
// 
//       Filename:  ipc_msg_queue.cpp
// 
//    Description:  IPC 消息队列 shm
// 
//        Version:  1.0
//        Created:  05/31/2009 02:16:25 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Will King (xfun), xfun68@foxmail.com
//        Company:  
// 
// =====================================================================================/*}}}*/

#include "ipc_msg_queue.h"/*{{{*/
#include "tool.h"
#include <new>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>/*}}}*/

// TODO LIST
// 解决push覆盖问题

IPCMsgQueue::IPCMsgQueue(bool push_side) :
    data_(NULL),/*{{{*/
    shmid_(-1),
    push_side_(push_side),
    def_msg_(0x10, 0x10001001, 16)
{
}/*}}}*/

IPCMsgQueue::~IPCMsgQueue(void)
{/*{{{*/
    release();
}/*}}}*/

int32_t IPCMsgQueue::initialize(uint32_t appID)
{
    int32_t result = E_ERROR;
    int32_t shmflg = 0;
    void* addr = NULL;

    key_t key = ftok(getenv("PWD"), appID);
    shmflg = IPC_CREAT | 0666;
    if ((shmid_ = shmget(key, sizeof(IPCMsgQueueData), shmflg)) == -1) {
        PUT_ERR("IPCMsgQueue::initialize::shmget");
        result = shmid_;
        goto ExitError;
    }
    if ((int32_t)(addr = shmat(shmid_, NULL, 0)) == -1) {
        PUT_ERR("IPCMsgQueue::initialize::shmat");
        result = (int32_t)addr;
        goto ExitError;
    }
    data_ = (IPCMsgQueueData*)addr;
    if (push_side_) {
        for (uint32_t i = 0; i < MAX_IPCMSG_QUEUE_SIZE; ++i) {
            new ((IPCMsg*)(data_->queue_) + i) SysProtocol(0x10, 
                0x10001001, AGIP_MAX_BUFFER_SIZE);
        }
        data_->head_ = data_->tail_ = 0;
    }

    result = S_SUCCESS;
ExitError:
    return result;
}

int32_t IPCMsgQueue::release(void)
{
    int32_t result = E_ERROR;

    if (NULL == data_) {
        goto ExitOK;
    }
    if (push_side_) {
        data_->head_ = data_->tail_ = 0;
    }
    shmid_ = -1;
    if (shmdt(data_) == -1) {
        PUT_ERR("IPCMsgQueue::release::shmdt");
    }
    data_ = NULL;

ExitOK:
    result = S_SUCCESS;
    return result;
}

IPCMsg& IPCMsgQueue::front(void)
{
    if (NULL == data_) {
        return def_msg_;
    }
    if (empty()) {
        return def_msg_;
    } else {
        return data_->queue_[data_->head_ % MAX_IPCMSG_QUEUE_SIZE];
    }
}

const IPCMsg& IPCMsgQueue::front(void) const
{
    if (NULL == data_) {
        return def_msg_;
    }
    if (empty()) {
        return def_msg_;
    } else {
        return data_->queue_[data_->head_ % MAX_IPCMSG_QUEUE_SIZE];
    }
}

IPCMsg& IPCMsgQueue::back(void)
{
    if (NULL == data_) {
        return def_msg_;
    }
    if (empty()) {
        return def_msg_;
    } else {
        return data_->queue_[(data_->tail_ - 1) % MAX_IPCMSG_QUEUE_SIZE];
    }
}

const IPCMsg& IPCMsgQueue::back(void) const
{
    if (NULL == data_) {
        return def_msg_;
    }
    if (empty()) {
        return def_msg_;
    } else {
        return data_->queue_[(data_->tail_ - 1) % MAX_IPCMSG_QUEUE_SIZE];
    }
}

IPCMsg& IPCMsgQueue::end(void)
{
    if (NULL == data_) {
        return def_msg_;
    }
    return data_->queue_[data_->tail_ % MAX_IPCMSG_QUEUE_SIZE];
}

const IPCMsg& IPCMsgQueue::end(void) const
{
    if (NULL == data_) {
        return def_msg_;
    }
    return data_->queue_[data_->tail_ % MAX_IPCMSG_QUEUE_SIZE];
}

bool IPCMsgQueue::empty(void) const
{
    if (NULL == data_) {
        return true;
    }
    return (data_->head_ == data_->tail_);
}

int32_t IPCMsgQueue::pop(void)
{
    if (NULL == data_) {
        return 0;
    }

    if (size() <= 0) {
        return 0;
    } else {
        ++data_->head_;
        return 1;
    }
}

int32_t IPCMsgQueue::push(const IPCMsg& val)
{
    if (NULL == data_) {
        return 0;
    }

    if (size() >= MAX_IPCMSG_QUEUE_SIZE) {
        return 0;
    }

    IPCMsg& end_msg = data_->queue_[data_->tail_ % MAX_IPCMSG_QUEUE_SIZE];
    if (&val != &end_msg) {
        end_msg = val;
    }
    ++data_->tail_;
    return 1;
}

uint32_t IPCMsgQueue::size(void) const
{
    if (NULL == data_) {
        return 0U;
    }
    return (uint32_t)(data_->tail_ - data_->head_);
}

uint64_t IPCMsgQueue::head(void) const
{
    if (NULL == data_) {
        return 0U;
    }

    return data_->head_;
}

uint64_t IPCMsgQueue::tail(void) const
{
    if (NULL == data_) {
        return 0U;
    }

    return data_->tail_;
}

