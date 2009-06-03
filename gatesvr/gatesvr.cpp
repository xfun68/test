// =====================================================================================/*{{{*/
// 
//       Filename:  gatesvr.cpp
// 
//    Description:  gatesvr main文件
// 
//        Version:  1.0
//        Created:  05/30/2009 05:54:54 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Will King (xfun), xfun68@foxmail.com
//        Company:  
// 
// =====================================================================================/*}}}*/

#include "OperationCode.h"/*{{{*/
#include "event_handler.h"
#include "connection_manager.h"
#include "tool.h"
#include "shm.h"
#include "loop_queue.h"
#include <signal.h>
#include <stdlib.h>
#include <strings.h>
#include <iostream>/*}}}*/

using namespace std;
using namespace socketor;

const int32_t DATA_LEN = 64;
struct DataUnit {
    int32_t len;
    int8_t data[DATA_LEN];
};

int32_t g_count = 100;

void sigExit(int signo) {/*{{{*/
    g_count = 0;
    return;
}/*}}}*/

class GateSvr : public EventHandler {
public:
    GateSvr(void) : 
        EventHandler("GateSvr"),
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
            puts("ConnectionManager initialize failed");
            goto ExitError;
        }
        puts("ConnectionManager initialize OK");/*}}}*/

        result = S_SUCCESS;
ExitError:
        return result;
    }

    int32_t release(void) {
        int32_t result = E_ERROR;

        // 释放资源
        if (S_SUCCESS != cm_.release()) {/*{{{*/
            puts("ConnectionManager release failed");
            result = -1;
        }
        puts("ConnectionManager release OK");/*}}}*/

        return result;
    }

    int32_t run(void) {
        int32_t result = E_ERROR;

        // 使 ConnectionManager 在单独的线程中运行
        if (0 != runInThread<ConnectionManager>(&cm_)) {/*{{{*/
            cout << "runInThread(ConnectionManager) failed" << endl;
            goto ExitError;
        }
        cout << "runInThread(ConnectionManager) OK" << endl;/*}}}*/

        result = S_SUCCESS;
ExitError:
        return result;
    }

    int32_t onBeginLoop(ConnectionManager* connMgr) {
        int32_t result = E_ERROR;

        EventHandler::onBeginLoop(connMgr);
        if (conn_ != NULL) {
            goto ExitOK;
        }
        // 创建监听连接
        if (S_SUCCESS != cm_.createConnection(0U, 12345, conn_)) {/*{{{*/
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

    int32_t onListenFailed(Connection* conn) {
        conn->set_auto_release(true);
        conn_ = NULL;
        return S_SUCCESS;
    }

    int32_t onRelease(Connection* conn) {
        conn->set_auto_release(true);
        if (conn_ == conn) {
            conn_ = NULL;
        }
        return S_SUCCESS;
    }

    int32_t onRead(Connection* conn) {
        int32_t retcode = -1;

        DataUnit& msg = shm_stoc_q.end();
        msg.len = 0;
        bzero(msg.data, DATA_LEN);
        conn->read((void*)&msg.data, DATA_LEN, msg.len);
        msg.data[DATA_LEN-1] = 0;

        retcode = shm_stoc_q.push(msg);
        printf("SHMQ:");
        printf(" head[%04u]", shm_stoc_q.head());
        printf(" tail[%04u]", shm_stoc_q.tail());
        printf(" size[%04u]", shm_stoc_q.size());
        printf(" addr[%p]", (void*)&msg);

        if (retcode < 0) {
            cout << " push failed ";
        } else {
            cout << " push   OK   ";
        }

        printf("===> %d:[%s] <===\n", msg.len, (char*)msg.data);

        return 0;
    }

private:
    uint8_t app_id_;
    uint32_t queue_capacity_;
    Connection* conn_;
    ConnectionManager cm_;
    LoopQueue<DataUnit> shm_stoc_q;
    LoopQueue<DataUnit> shm_ctos_q;
};

int main (int argc, char *argv[])
{/*{{{*/
    int32_t result = -1;
    int32_t retcode = -1;
    GateSvr gateSvr;

    instanceRestrict(argv[0]);/*{{{*/
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, sigExit);
    signal(SIGINT, sigExit);
    signal(SIGTERM, sigExit);/*}}}*/

    if (S_SUCCESS != (retcode = gateSvr.initialize())) {
        printf("GateSvr initialize failed with error [%d]\n",
            retcode);
        goto ExitError;
    }
    printf("GateSvr initialize OK\n");

    if (S_SUCCESS != (retcode = gateSvr.run())) {
        printf("GateSvr startup failed with error [%d]\n",
            retcode);
        goto ExitError;
    }
    printf("GateSvr startup OK\n");

    while (g_count > 0) {
        sleep(1);
    }

    result = 0;
ExitError:

    if (S_SUCCESS != (retcode = gateSvr.release())) {
        printf("GateSvr release failed with error [%d]\n",
            retcode);
    }
    printf("GateSvr release OK\n");

    if (0 == result) {/*{{{*/
        puts("program exit OK");
    } else {
        printf("program exit with error[%d]", result);
    }/*}}}*/
    return result;
}				// ----------  end of function main  ----------/*}}}*/

