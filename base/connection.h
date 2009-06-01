#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "sockfd.h"
#include "sockaddr_in.h"
#include "event_handler.h"
#include "connection_manager.h"
#include "app_buffer.h"
#include "OperationCode.h"

#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>          // for recv
#include <sys/socket.h>         // for recv, send
#include <sys/epoll.h>          // for epoll

#include <inttypes.h>

// TODO LIST
// *** ���ֳ���ͬ�����ӹر��¼�������OnClose��OnDisconnect����OnError��OnHup�ǲ�ͬ��
// # should it surport auto reconnect
// # mv error codes below into OperationCode.h
// # �Ƿ���Ҫ�����������ͣ������������ӣ��������ӣ���������

namespace socketor {
    const int32_t E_SYS_INVALID_HANDLER = -901; // ��Ч�Ĵ�����
    const int32_t E_SYS_ERROR_STATE = -902; // ����Ӧ�����˴����״̬��
    const int32_t E_NOT_CONN = -903; // ����δ����
    typedef struct epoll_event Event;

    class Connection
    {
    public:
        friend class ConnectionManager;

        enum {
            BUF_SIZE = 1024 };
        enum State {
            CLOSED = 1, // default state
            CLOSE_WAIT,
            LISTEN,
            CONNECT_SENT,
            ESTABLISHED };

        // operation --------------------------------------------------
        int32_t listen(void);
        int32_t accept(void);
        int32_t connect(void);
        int32_t disconnect(void);
        int32_t read(void* buffer,
            int32_t want_read_bytes,
            int32_t& real_read_bytes);
        int32_t peek(void* buffer,
            int32_t want_read_bytes,
            int32_t& real_read_bytes);
        int32_t seek(int32_t bytes);
        int32_t peekUInt16(uint16_t& val);
        int32_t write(const int8_t* buffer, 
            int32_t want_write_bytes,
            int32_t& real_write_bytes);

        // data member accessor
        const char* string_ip(void) const;
        int32_t set_ip(const char* string_ip);

        uint32_t numeric_ip(void) const;
        int32_t set_ip(uint32_t numeric_ip);

        uint16_t port(void) const;
        int32_t set_port(uint16_t port);

        const char* protocol(void) const;
        int32_t set_protocol(const char* protocol_string);

        EventHandler* event_handler(void) const;
        int32_t set_event_handler(EventHandler* event_handler);

        uint32_t events(void) const;
        int32_t set_events(uint32_t events = 
            EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLERR | EPOLLHUP);

        struct epoll_event epollevent(void) const;

        Sockfd& sockfd(void);
        const Sockfd& sockfd(void) const;

        State state(void) const;
        bool auto_release(void) const;
        int32_t set_auto_release(bool auto_release);
        // const ConnectionManager * const connection_manager(void) const;
        ConnectionManager* connection_manager(void);

    private:
        // constructor & destructor & operators -----------------------
        Connection(void);
        Connection(uint32_t numeric_ip,
            uint16_t port,
            EventHandler* event_handler = NULL);
        Connection(const char* string_ip,
            uint16_t port,
            EventHandler* event_handler = NULL);
        ~Connection(void);

        Connection(const Connection& other);
        const Connection& operator = (const Connection& other);

        // method -----------------------------------------------------
        int32_t initialize(void);
        /*
         *  ���ܣ�  ��ָ�����׽��ֺ͵�ַ��ʼ��һ��tcp����
         *  Լ����  ���������������׽���;
         *          ������سɹ����������Ѵ��ڡ�ESTABLISHED��״̬��
         *           # �������������ݵķ��ͺͽ���;
         *          �����˺�����ʼ������������ڡ����������ӡ���stateΪ
         *           # CLOSED ʱ���ᱻConnectionManager�Զ�����;
         */
        int32_t initialize(in_addr_t sockfd,
            struct sockaddr_in sain);
        /*
         * ���ܣ�   �ͷ�����ռ�õ���Դ������socket��
         *          ��epoll��ɾ����Ӧ��sockfd
         * ���أ�   1 �ɹ�
         * Լ����   ���ý�����ǳɹ���
         */
        int32_t recv(void);
        int32_t send(void);
        int32_t release(void);
        int32_t set_state(State);
        int32_t set_connection_manager(ConnectionManager* const connection_manager);
        struct epoll_event* pepollevent(void);
        int32_t onEvents(const Event& event);

        int32_t init_ok_;
        bool auto_release_;
        State state_;
        EventHandler default_event_handler_;
        ConnectionManager* connection_manager_;
        EventHandler* event_handler_;
        SockaddrIn sain_;
        Sockfd sockfd_;
        Event event_;
        AppBuffer recv_buf_;
        AppBuffer send_buf_;
    };

} // end of namespace socketor

#endif // _CONNECTION_H_

