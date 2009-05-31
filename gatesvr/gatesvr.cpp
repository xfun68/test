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
#include "ipc_msg_queue.h"

#include <signal.h>
#include <stdlib.h>
#include <strings.h>
#include <iostream>/*}}}*/

using namespace std;
using namespace socketor;

int32_t g_count = 100;

void sigExit(int signo) {/*{{{*/
    g_count = 0;
    return;
}/*}}}*/

class GateSvr : public EventHandler {
public:
    GateSvr(void) : 
        EventHandler("GateSvr"),
        conn(NULL),
        ipc_stoc_q(1),
        ipc_ctos_q(0),
        msg_count_(0) { }

    int32_t initialize(void) {
        int32_t result = E_ERROR;

        // 初始化资源
        if ((S_SUCCESS != ipc_stoc_q.initialize(0x10)) ||/*{{{*/
            ((S_SUCCESS != ipc_ctos_q.initialize(0x11)))) {
            puts("IPCMsgQueue initialize failed");
            goto ExitError;
        }
        puts("IPCMsgQueue initialize OK");/*}}}*/

        if (S_SUCCESS != cm.initialize()) {/*{{{*/
            puts("ConnectionManager initialize failed");
            goto ExitError;
        }
        puts("ConnectionManager initialize OK");/*}}}*/

ExitError:
        return result;
    }

    int32_t release(void) {
        int32_t result = E_ERROR;

        // 释放资源
        if (S_SUCCESS != cm.release()) {/*{{{*/
            puts("ConnectionManager release failed");
            result = -1;
        }
        puts("ConnectionManager release OK");/*}}}*/

        if ((S_SUCCESS != ipc_stoc_q.release()) ||/*{{{*/
            ((S_SUCCESS != ipc_ctos_q.release()))) {
            puts("IPCMsgQueue release failed");
            goto ExitError;
        }
        puts("IPCMsgQueue release OK");/*}}}*/

ExitError:
        return S_SUCCESS;
    }

    int32_t run(void) {
        int32_t result = E_ERROR;

        // 使 ConnectionManager 在单独的线程中运行
        if (0 != runInThread<ConnectionManager>(&cm)) {/*{{{*/
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
        if (conn != NULL) {
            goto ExitOK;
        }
        // 创建监听连接
        if (S_SUCCESS != cm.createConnection(0U, 12345, conn)) {/*{{{*/
            printf("create connection failed\n");
            goto ExitError;
        }
        printf("create connection OK\n");

        if (S_SUCCESS != conn->listen()) {
            printf("listen at port %u failed\n", conn->port());
            goto ExitError;
        }
        printf("listen at port %u OK\n", conn->port());/*}}}*/

ExitOK:
        result = S_SUCCESS;
ExitError:
        return S_SUCCESS;
    }

    int32_t onRead(Connection* conn) {
        EventHandler::onRead(conn);
        IPCMsg& msg = ipc_stoc_q.end();
        msg.setSequenceID(++msg_count_);
        ipc_stoc_q.push(msg);
        return 1;
    }

private:
    Connection* conn;
    IPCMsgQueue ipc_stoc_q;
    IPCMsgQueue ipc_ctos_q;
    int32_t msg_count_;
    ConnectionManager cm;
};

int main (int argc, char *argv[])
{
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
}				// ----------  end of function main  ----------

