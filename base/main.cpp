// =====================================================================================/*{{{*/
// 
//       Filename:  main.cpp
// 
//    Description:  测试 libsocketor.so
// 
//        Version:  1.0
//        Created:  05/24/2009 09:56:38 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Will King (xfun), xfun68@foxmail.com
//        Company:  
// 
// =====================================================================================
/*}}}*/
#include "event_handler.h"
#include "connection_manager.h"
#include "tool.h"

#include <signal.h>
#include <stdlib.h>
#include <strings.h>
#include <iostream>

using namespace std;
using namespace socketor;

int32_t g_count = 100;

void sigExit(int signo) {/*{{{*/
    g_count = 0;
    return;
}/*}}}*/

int main (int argc, char *argv[])
{
    int32_t result = -1;
    // 声明
    Connection* conn;
    ConnectionManager cm;

    signal(SIGHUP, SIG_IGN);/*{{{*/
    signal(SIGPIPE, sigExit);
    signal(SIGINT, sigExit);
    signal(SIGTERM, sigExit);/*}}}*/

    // 初始化 ConnectionManager
    if (S_SUCCESS != cm.initialize()) {
        puts("ConnectionManager initialize failed");
        goto ExitError;
    }
    puts("ConnectionManager initialize OK");

    // 创建监听连接
    if (S_SUCCESS != cm.createConnection(0U, 12345, conn)) {
        printf("create connection failed\n");
        goto ExitError;
    }
    printf("create connection OK\n");

    if (S_SUCCESS != conn->listen()) {
        printf("listen at port %u failed\n", conn->port());
        goto ExitError;
    }
    printf("listen at port %u OK\n", conn->port());

    // 使 ConnectionManager 在单独的线程中运行
    if (0 != runInThread<ConnectionManager>(&cm)) {
        cout << "runInThread(ConnectionManager) failed" << endl;
        goto ExitError;
    }
    cout << "runInThread(ConnectionManager) OK" << endl;

    while (g_count > 0) {
        sleep(1);
    }

    result = 0;
ExitError:
    // 释放 ConnectionManager
    if (S_SUCCESS != cm.release()) {
        puts("ConnectionManager release failed");
        result = -1;
    }
    puts("ConnectionManager release OK");

    if (0 == result) {
        puts("program exit OK");
    } else {
        printf("program exit with error[%d]", result);
    }
    return result;
}				// ----------  end of function main  ----------

