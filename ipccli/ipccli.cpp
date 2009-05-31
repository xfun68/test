// =====================================================================================/*{{{*/
// 
//       Filename:  cli.cpp
// 
//    Description:  cli main文件，一个使用socktor实现的简易tcp客户端demo
// 
//        Version:  1.0
//        Created:  05/30/2009 05:54:54 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Will King (xfun), xfun68@foxmail.com
//        Company:  
// 
// =====================================================================================
/*}}}*/
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

int32_t g_count = 100;

void sigExit(int signo) {/*{{{*/
    g_count = 0;
    return;
}/*}}}*/

int main (int argc, char *argv[])
{
    // 声明
    int32_t result = -1;
    IPCMsgQueue ipc_stoc_q(0);
    IPCMsgQueue ipc_ctos_q(1);

    // 进程唯一约束、设置信号处理函数
    instanceRestrict(argv[0]);/*{{{*/
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, sigExit);
    signal(SIGINT, sigExit);
    signal(SIGTERM, sigExit);/*}}}*/

    // 初始化 IPCMsgQueue
    if ((S_SUCCESS != ipc_stoc_q.initialize(0x10)) ||/*{{{*/
       ((S_SUCCESS != ipc_ctos_q.initialize(0x11)))) {
        puts("IPCMsgQueue initialize failed");
        goto ExitError;
    }
    puts("IPCMsgQueue initialize OK");/*}}}*/


    // 控制主线程是否结束
    while (g_count > 0) {/*{{{*/
        sleep(1);
    }/*}}}*/

    result = 0;
ExitError:
    // 释放 IPCMsgQueue
    if ((S_SUCCESS != ipc_stoc_q.release()) ||/*{{{*/
       ((S_SUCCESS != ipc_ctos_q.release()))) {
        puts("IPCMsgQueue release failed");
        goto ExitError;
    }
    puts("IPCMsgQueue release OK");/*}}}*/

    if (0 == result) {/*{{{*/
        puts("program exit OK");
    } else {
        printf("program exit with error[%d]", result);
    }/*}}}*/
    return result;
}				// ----------  end of function main  ----------

