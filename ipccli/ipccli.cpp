// =====================================================================================/*{{{*/
// 
//       Filename:  cli.cpp
// 
//    Description:  cli main�ļ���һ��ʹ��socktorʵ�ֵļ���tcp�ͻ���demo
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
#include "loop_queue.h"
#include "shm.h"

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
    // ����
    int32_t result = -1;
    int32_t shmid = -1;
    void* addr = NULL;
    LoopQueue<DataUnit> shm_stoc_q;
    LoopQueue<DataUnit> shm_ctos_q;

    // ����ΨһԼ���������źŴ�����
    instanceRestrict(argv[0]);/*{{{*/
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, sigExit);
    signal(SIGINT, sigExit);
    signal(SIGTERM, sigExit);/*}}}*/

    // ��ʼ��
    if ((shmid = shmGet(g_app_id+2)) < 0) {/*{{{*/
        goto ExitError;
    }
    printf("SHMID: %d\n", shmid);
    if ((addr = shmAt(shmid)) < 0) {
        goto ExitError;
    }
    printf("SHMADDR: %p\n", addr);
    if (shm_stoc_q.setQueueAddr(addr) < 0) {
        goto ExitError;
    }

    if ((shmid = shmGet(g_app_id+1)) < 0) {
        goto ExitError;
    }
    printf("SHMID: %d\n", shmid);
    if ((addr = shmAt(shmid)) < 0) {
        goto ExitError;
    }
    printf("SHMADDR: %p\n", addr);
    if (shm_ctos_q.setQueueAddr(addr) < 0) {
        goto ExitError;
    }
    puts("SHM initialize OK");/*}}}*/

    while (g_count > 0) {/*{{{*/
        if (shm_stoc_q.size() > 0) {
            printf("SHMQ:");
            printf(" head[%04u]", shm_stoc_q.head());
            printf(" tail[%04u]", shm_stoc_q.tail());
            printf(" size[%04u]", shm_stoc_q.size());
            DataUnit& msg = shm_stoc_q.front();
            printf(" addr[%p]", (void*)&msg);
            if (shm_stoc_q.pop() < 0) {
                printf(" pop failed ");
            } else {
                printf(" pop   OK   ");
            }
            printf("===> %d:[%s] <===\n", msg.len, (char*)msg.data);
        }

        usleep(300000);
    }/*}}}*/

    result = 0;
ExitError:
    if (0 == result) {/*{{{*/
        puts("program exit OK");
    } else {
        printf("program exit with error[%d]", result);
    }/*}}}*/
    return result;
}				// ----------  end of function main  ----------

