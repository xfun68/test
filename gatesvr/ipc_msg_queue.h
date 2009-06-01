// =====================================================================================/*{{{*/
// 
//       Filename:  ipc_msg_queue.h
// 
//    Description:  IPC 消息队列 shm
// 
//        Version:  1.0
//        Created:  05/31/2009 02:16:57 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Will King (xfun), xfun68@foxmail.com
//        Company:  
// 
// =====================================================================================
/*}}}*/
#ifndef _IPC_MSG_QUEUE_H_
#define _IPC_MSG_QUEUE_H_

#include "SysProtocol.h"
#include <inttypes.h>

const uint32_t MAX_IPCMSG_DATA_SIZE = 1024;
const uint32_t MAX_IPCMSG_QUEUE_SIZE = 1024;

struct IPCMsg {
    int8_t data[MAX_IPCMSG_DATA_SIZE];
};

struct IPCMsgQueueData {
    uint64_t head_;
    uint64_t tail_;
    IPCMsg queue_[MAX_IPCMSG_QUEUE_SIZE];
};

class IPCMsgQueue {
public:
    explicit IPCMsgQueue(bool push_side);
    ~IPCMsgQueue(void);

    int32_t initialize(uint32_t appID);
    int32_t release(void);

    IPCMsg& front(void);
    IPCMsg& back(void);
    IPCMsg& end(void);
    const IPCMsg& back(void) const;
    const IPCMsg& end(void) const;
    const IPCMsg& front(void) const;

    int32_t pop(void);
    int32_t push(const IPCMsg& val);

    bool empty(void) const;
    uint32_t size(void) const;
    uint64_t head(void) const;
    uint64_t tail(void) const;

private:
    IPCMsgQueue(void);
    IPCMsgQueue(const IPCMsgQueue& rhs);
    IPCMsgQueue& operator = (const IPCMsgQueue& rhs);

    IPCMsgQueueData* data_;
    int32_t shmid_;
    bool push_side_;
    IPCMsg def_msg_;
};

#endif //  _IPC_MSG_QUEUE_H_

