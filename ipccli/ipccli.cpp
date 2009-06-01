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
#include "shm_msg_queue.h"

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
    SHMMsgQueue shm_stoc_q(0);
    SHMMsgQueue shm_ctos_q(1);

    // 进程唯一约束、设置信号处理函数
    instanceRestrict(argv[0]);/*{{{*/
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, sigExit);
    signal(SIGINT, sigExit);
    signal(SIGTERM, sigExit);/*}}}*/

    // 初始化 SHMMsgQueue
    if ((S_SUCCESS != shm_stoc_q.initialize(0x10)) ||/*{{{*/
       ((S_SUCCESS != shm_ctos_q.initialize(0x11)))) {
        puts("SHMMsgQueue initialize failed");
        goto ExitError;
    }
    puts("SHMMsgQueue initialize OK");/*}}}*/

    while (g_count > 0) {/*{{{*/
        printf("SHMMsgQueue size: %u\n", shm_stoc_q.size());
        while (shm_stoc_q.size() > 0) {
            SHMMsg& msg = shm_stoc_q.front();
            if (shm_stoc_q.pop() < 0) {
                printf("SHMMsgQueue pop failed ");
                continue;
            } else {
                printf("SHMMsgQueue pop OK ");
            }
            cout << "head: " << shm_stoc_q.head()
                << " tail: " << shm_stoc_q.tail()
                << " size: " << shm_stoc_q.size()
                << endl;
            snprintf("SHMCli read ipc msg [%s]\n", msg.len_, (char*)msg.data_);
        }
        sleep(1);
    }/*}}}*/

    result = 0;
ExitError:
    // 释放 SHMMsgQueue
    if ((S_SUCCESS != shm_stoc_q.release()) ||/*{{{*/
       ((S_SUCCESS != shm_ctos_q.release()))) {
        puts("SHMMsgQueue release failed");
        goto ExitError;
    }
    puts("SHMMsgQueue release OK");/*}}}*/

    if (0 == result) {/*{{{*/
        puts("program exit OK");
    } else {
        printf("program exit with error[%d]", result);
    }/*}}}*/
    return result;
}				// ----------  end of function main  ----------

