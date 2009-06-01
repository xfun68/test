/*  
 *  现在默认构造的是tcp类型的socket描述符，暂时不支持udp或其它类型的
 *    socket描述符。
 */
#ifndef _SOCKFD_H_
#define _SOCKFD_H_

#include "OperationCode.h"

#include <sys/types.h>      // for sa_family_t, sa_port_t, int32_t ...
#include <netinet/in.h>     // in_addr_t
#include <sys/socket.h>
#include <fcntl.h>          // for set nonblock fd

namespace socketor {
    class Sockfd
    {
    public:
        // constructors
        Sockfd(void);
        // destructors
        ~Sockfd(void);

        // getter and setter
        sa_family_t family(void) const;
        int32_t set_family(sa_family_t new_family);

        int32_t type(void) const;
        int32_t set_type(int32_t new_type);

        int32_t protocol(void) const;
        int32_t set_protocol(int32_t new_protocol);

        in_addr_t sockfd(void) const;

        // operators
        int32_t initialize(void);
        int32_t initialize(in_addr_t sockfd);
        int32_t release(void);
        int32_t isError(void);
        int32_t setNonBlocking(void);
        int32_t setReuseAddr(void);
        int32_t setLinger(int32_t on = 1, int32_t to = 0);
        /*
         * 功能：   获取当前系统允许的套接字最大缓冲区大小
         * 返回：
         *  # -1    失败
         *  # >0    缓冲区的大小
         */
        long getSockMaxBuf(void);
        /*
         * 功能：   获取套接字当前的发送缓冲区大小
         * 返回：
         *  # >0    缓冲区的大小
         *  # -1    失败
         */
        int32_t getSendBufSize(void);
        /*
         * 功能：   获取套接字当前的接收缓冲区大小
         * 返回：
         *  # >0    缓冲区的大小
         *  # -1    失败
         */
        int32_t getRecvBufSize(void);
        /*
         * 功能：   设置套接字当前的发送缓冲区大小
         * 返回：
         *  # 0     成功
         *  # -1    失败
         */
        int32_t setSendBufSize(int32_t size);
        /*
         * 功能：   设置套接字当前的接收缓冲区大小
         * 返回：
         *  # 0     成功
         *  # -1    失败
         */
        int32_t setRecvBufSize(int32_t size);

        // type cast operators
        // operator in_addr_t(void) const;
        operator int32_t(void) const;
        operator uint32_t(void) const;

    private:
        Sockfd(const Sockfd& rhs);
        Sockfd& operator = (const Sockfd& rhs);
        const Sockfd& operator = (const Sockfd& rhs) const;

    private:
        sa_family_t family_;
        __socket_type type_;
        int32_t protocol_;
        in_addr_t sockfd_;
    };
} // end of namespace socketor
#endif //  _SOCKFD_H_

