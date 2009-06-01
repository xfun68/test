#ifndef _CONNECTION_MANAGER_H_
#define _CONNECTION_MANAGER_H_

/* user defined header file */
#include "connection.h"
#include "event_handler.h"
#include "OperationCode.h"
/* system header file */
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>
#include <sys/epoll.h>
/* c/cpp lib header file */
#include <stdio.h>
#include <list>

const uint64_t USEC_PER_SEC = 1000 * 1000;

namespace socketor
{
    class ConnectionManager
    {
    public:
        friend class Connection;
        typedef std::list<Connection*> ConnectionList;
        typedef std::list<Connection*>::iterator ConnectionListItr;

        enum {
            MAX_CONNECTION_NUM = 1024
        };

    public:
        ConnectionManager(EventHandler* event_handler = NULL);
        ~ConnectionManager(void);

        int32_t initialize(void);
        int32_t release(void);

        int32_t setEventHandler(EventHandler* event_handler);
        int32_t getEventHandler(EventHandler*& event_handler);

        int32_t createConnection(uint32_t ip,
            uint16_t port,
            Connection*& conn);
        int32_t createConnection(const char* ip,
            uint16_t port,
            Connection*& conn);
        int32_t createConnection(Connection*& conn);
        int32_t createPollessConnection(Connection*& conn);
        int32_t releaseConnection(Connection* conn);
        int32_t controlAdd(Connection* const conn);
        /*
         * 约束：   无论返回何值，总保证epoll轮询列表内已无
         *          conn->sockfd()所指定的套接字
         */
        int32_t controlDel(Connection* conn);
        int32_t run(void);
        int32_t getConnctionNumber(void);

    private:
        int32_t initializeConnectionList(void);
        int32_t releaseConnectionList(void);

    private:
        int32_t epfd_;
        int32_t init_ok_;
        EventHandler default_event_handler_;
        EventHandler* event_handler_;
        struct epoll_event* event_q_;
        ConnectionList conn_list_;
    };

} // end of namespace socketor

#endif // _CONNECTION_MANAGER_H_

