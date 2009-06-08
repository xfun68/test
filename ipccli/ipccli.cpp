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
#include "shmq.h"

#include <signal.h>
#include <stdlib.h>
#include <strings.h>
#include <iostream>/*}}}*/

using namespace std;

int32_t g_count = 100;
uint8_t g_app_id = 0x10;
const int32_t DATA_LEN = 64;
struct DataUnit {
    int32_t len;
    int8_t data[DATA_LEN];
};

void sigExit(int signo) {/*{{{*/
    g_count = 0;
    return;
}/*}}}*/

int main (int argc, char *argv[])
{
    // 声明
    int32_t result = -1;
    SHMQ<DataUnit> shm_stoc_q;
    SHMQ<DataUnit> shm_ctos_q;

    // 进程唯一约束、设置信号处理函数
    instanceRestrict(argv[0]);/*{{{*/
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, sigExit);
    signal(SIGINT, sigExit);
    signal(SIGTERM, sigExit);/*}}}*/

    // 初始化
    if (shm_stoc_q.initialize(0x10, 0x20, 1024, false) < 0) {
        goto ExitError;
    }

    if (shm_ctos_q.initialize(0x20, 0x10, 16, true) < 0) {
        goto ExitError;
    }

    while (g_count > 0) {/*{{{*/
        if (shm_stoc_q.queue().size() > 0) {
            printf("SHMQ:");
            printf(" head[%04u]", shm_stoc_q.queue().head());
            printf(" tail[%04u]", shm_stoc_q.queue().tail());
            printf(" size[%04u]", shm_stoc_q.queue().size());
            DataUnit& msg = shm_stoc_q.queue().front();
            printf(" addr[%p]", (void*)&msg);
            if (shm_stoc_q.queue().pop() < 0) {
                printf(" pop failed ");
            } else {
                printf(" pop   OK   ");
            }
            printf("---> %d:[%s]\n", msg.len, (char*)msg.data);
        }

        usleep(300000);
    }/*}}}*/

    result = 0;
ExitError:
    if (0 != shm_stoc_q.release()) {/*{{{*/
        puts("shm_stoc_q release failed");
    }
    puts("shm_stoc_q release OK");/*}}}*/

    if (0 != shm_ctos_q.release()) {/*{{{*/
        puts("shm_ctos_q release failed");
    }
    puts("shm_ctos_q release OK");/*}}}*/

    if (0 == result) {/*{{{*/
        puts("program exit OK");
    } else {
        printf("program exit with error[%d]", result);
    }/*}}}*/
    return result;
}				// ----------  end of function main  ----------

