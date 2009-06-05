#ifndef __GATE_H
#define __GATE_H

#include "event_handler.h"
#include "tool.h"
#include "shm.h"
#include "loop_queue.h"
#include "connection_manager.h"

const int32_t DATA_LEN = 64;
struct DataUnit {
    int32_t len;
    int8_t data[DATA_LEN];
};

class Gate : public socketor::EventHandler {
public:
    Gate(void) : 
        EventHandler("Gate"),
        app_id_(0x10),
        queue_capacity_(1024),
        conn_(NULL),
        cm_(this) {
    }

    int32_t initialize(void) {
        int32_t result = E_ERROR;
        int32_t shmid = -1;
        void* addr = NULL;

        // 初始化资源
        if ((shmid = shmCreate(app_id_ + 2, sizeof(DataUnit) * queue_capacity_)) < 0) {
            goto ExitError;
        }
        printf("SHMID: %d\n", shmid);
        if ((addr = shmAt(shmid)) < 0) {
            goto ExitError;
        }
        printf("SHMADDR: %p\n", addr);
        if (shm_stoc_q.setQueueAddr(addr, sizeof(DataUnit), queue_capacity_) < 0) {
            goto ExitError;
        }

        if ((shmid = shmCreate(app_id_ + 1, sizeof(DataUnit) * queue_capacity_)) < 0) {
            goto ExitError;
        }
        printf("SHMID: %d\n", shmid);
        if ((addr = shmAt(shmid)) < 0) {
            goto ExitError;
        }
        printf("SHMADDR: %p\n", addr);
        if (shm_ctos_q.setQueueAddr(addr, sizeof(DataUnit), queue_capacity_) < 0) {
            goto ExitError;
        }

        if (S_SUCCESS != cm_.initialize()) {/*{{{*/
            puts("socketor::ConnectionManager initialize failed");
            goto ExitError;
        }
        puts("socketor::ConnectionManager initialize OK");/*}}}*/

        result = S_SUCCESS;
ExitError:
        return result;
    }

    int32_t release(void) {
        int32_t result = -1;

        // 释放资源
        if (S_SUCCESS != cm_.release()) {/*{{{*/
            puts("socketor::ConnectionManager release failed");
        }
        puts("socketor::ConnectionManager release OK");/*}}}*/

        result = 0;
        return result;
    }

    int32_t run(void) {
        int32_t result = E_ERROR;

        if (0 != runInThread<socketor::ConnectionManager>(&cm_)) {/*{{{*/
            printf("runInThread(socketor::ConnectionManager) failed\n");
            goto ExitError;
        }
        printf("runInThread(socketor::ConnectionManager) OK\n");/*}}}*/

        result = S_SUCCESS;
ExitError:
        return result;
    }

    int32_t onBeginLoop(socketor::ConnectionManager* connMgr) {
        int32_t result = E_ERROR;

        EventHandler::onBeginLoop(connMgr);
        if (conn_ != NULL) {
            goto ExitOK;
        }
        // 创建监听连接
        if (S_SUCCESS != cm_.createConnection(0U, 8000, conn_)) {/*{{{*/
            printf("create connection failed\n");
            goto ExitError;
        }
        printf("create connection OK\n");
        conn_->set_event_handler(this);

        if (S_SUCCESS != conn_->listen()) {
            printf("listen failed\n");
            goto ExitError;
        }
        printf("listen at port %u OK\n", conn_->port());/*}}}*/

ExitOK:
        result = S_SUCCESS;
ExitError:
        return S_SUCCESS;
    }

    int32_t onListenFailed(socketor::Connection* conn) {
        conn->set_auto_release(true);
        conn_ = NULL;
        return S_SUCCESS;
    }

    int32_t onRelease(socketor::Connection* conn) {
        conn->set_auto_release(true);
        if (conn_ == conn) {
            conn_ = NULL;
        }
        return S_SUCCESS;
    }

    int32_t onRead(socketor::Connection* conn) {
        int32_t retcode = -1;

        DataUnit& msg = shm_stoc_q.end();
        msg.len = 0;
        bzero(msg.data, DATA_LEN);
        conn->read((void*)&msg.data, DATA_LEN, msg.len);
        msg.data[DATA_LEN-1] = 0;
        if (msg.len <= 0) {
            return 0;
        }

        retcode = shm_stoc_q.push(msg);
        printf("SHMQ:");
        printf(" head[%04u]", shm_stoc_q.head());
        printf(" tail[%04u]", shm_stoc_q.tail());
        printf(" size[%04u]", shm_stoc_q.size());
        printf(" addr[%p]", (void*)&msg);

        if (retcode < 0) {
            puts(" push failed ");
        } else {
            puts(" push   OK   ");
        }

        printf("===> %d:[%s] <===\n", msg.len, (char*)msg.data);

        return 0;
    }

private:
    uint8_t app_id_;
    uint32_t queue_capacity_;
    socketor::Connection* conn_;
    socketor::ConnectionManager cm_;
    LoopQueue<DataUnit> shm_stoc_q;
    LoopQueue<DataUnit> shm_ctos_q;
};

#endif //  __GATE_H

