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

#include <signal.h>
#include <stdlib.h>
#include <strings.h>
#include <iostream>/*}}}*/

using namespace std;/*{{{*/
using namespace socketor;/*}}}*/

int32_t g_count = 100;

void sigExit(int signo) {/*{{{*/
    g_count = 0;
    return;
}/*}}}*/

class CliHandler : public EventHandler
{/*{{{*/
public:
    CliHandler(std::string name) :
        EventHandler(name),/*{{{*/
        count_(0),
        conn_(NULL) {
    }/*}}}*/

    int32_t onBeginLoop(ConnectionManager* connMgr) {
        EventHandler::onBeginLoop(connMgr);/*{{{*/
        if (NULL == conn_) {
            // 创建连接
            if (S_SUCCESS != connMgr->createConnection("127.0.0.1", 12345, conn_)) {
                printf("create connection failed\n");
                return S_SUCCESS;
            }
            printf("create connection OK\n");
            conn_->set_event_handler(this);
            conn_->connect();
            printf("connect %s %u\n", conn_->string_ip(), conn_->port());
        } else if (conn_->state() != Connection::ESTABLISHED) {
            conn_->connect();
            printf("connect %s %u\n", conn_->string_ip(), conn_->port());
        }
        return S_SUCCESS;
    }/*}}}*/

    int32_t onWrite(Connection* conn) {
        const int32_t BUF_SIZE = 64;/*{{{*/
        int32_t sent_bytes = 0;
        int8_t buf[BUF_SIZE] = {0};

        snprintf((char*)buf, BUF_SIZE, "cli count: %u", ++count_);
        conn->write(buf, strlen((char*)buf), sent_bytes);
        printf("<--- %d:[%s]\n", sent_bytes, buf);
        return S_SUCCESS;
    }/*}}}*/

    int32_t onRelease(Connection* conn) {
        EventHandler::onRelease(conn);
        conn_ = NULL;/*{{{*/
        printf("connect failed\n");
        return S_SUCCESS;
    }/*}}}*/

private:
    uint32_t count_;
    Connection* conn_;
};/*}}}*/

int main (int argc, char *argv[])
{/*{{{*/
    // 声明
    int32_t result = -1;/*{{{*/
    CliHandler ch("CliHandler");
    ConnectionManager cm(&ch);/*}}}*/

    instanceRestrict(argv[0]);
    signal(SIGHUP, SIG_IGN);/*{{{*/
    signal(SIGPIPE, sigExit);
    signal(SIGINT, sigExit);
    signal(SIGTERM, sigExit);/*}}}*/

    // 1、初始化 ConnectionManager
    if (S_SUCCESS != cm.initialize()) {/*{{{*/
        puts("ConnectionManager initialize failed");
        goto ExitError;
    }
    puts("ConnectionManager initialize OK");/*}}}*/

    // 2、设置自己的处理器
    // cm.setEventHandler(&ch);

    // 3、使 ConnectionManager 在单独的线程中运行
    if (0 != runInThread<ConnectionManager>(&cm)) {/*{{{*/
        cout << "runInThread(ConnectionManager) failed" << endl;
        goto ExitError;
    }
    cout << "runInThread(ConnectionManager) OK" << endl;/*}}}*/

    // 控制主线程是否结束
    while (g_count > 0) {/*{{{*/
        sleep(1);
    }/*}}}*/

    result = 0;
ExitError:
    // 4、释放 ConnectionManager
    if (S_SUCCESS != cm.release()) {/*{{{*/
        puts("ConnectionManager release failed");
        result = -1;
    }
    puts("ConnectionManager release OK");/*}}}*/

    if (0 == result) {/*{{{*/
        puts("program exit OK");
    } else {
        printf("program exit with error[%d]", result);
    }/*}}}*/
    return result;
}				// ----------  end of function main  ----------/*}}}*/

