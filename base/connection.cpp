#include "connection.h"

#include <string.h>
#include <stdio.h>
#include <iostream>

#define ASSURE_SUCCESS(RESULT, RETCODE, STATEMENT) \
    if (S_SUCCESS != (RETCODE = (STATEMENT))) {\
        RESULT = RETCODE;\
        goto ExitError;\
    }

using namespace std;

namespace socketor
{

// TODO LIST
// 任何一个函数都可以也应该可以被多次调用而能够保证系统的正常

Connection::Connection(void) :
    init_ok_(0),
    auto_release_(0),
    state_(CLOSED),
    default_event_handler_("ConnDefHandler"),
    connection_manager_(NULL),
    event_handler_(&default_event_handler_)
{
    set_ip(0U);
    set_port(0U);
    set_protocol("tcp");
}

Connection::Connection(
    uint32_t numeric_ip,
    uint16_t port,
    EventHandler* event_handler) :
    init_ok_(0),
    auto_release_(0),
    state_(CLOSED),
    default_event_handler_("ConnDefHandler"),
    connection_manager_(NULL)
{
    set_ip(numeric_ip);
    set_port(port);
    set_protocol("tcp");

    if (NULL == event_handler) {
        event_handler_ = &default_event_handler_;
    } else {
        event_handler_ = event_handler;
    }
}

Connection::Connection(
    const char* string_ip,
    uint16_t port,
    EventHandler* event_handler) :
    init_ok_(0),
    auto_release_(0),
    state_(CLOSED),
    default_event_handler_("ConnDefHandler"),
    connection_manager_(NULL)
{
    set_ip(string_ip);
    set_port(port);
    set_protocol("tcp");

    if (NULL == event_handler) {
        event_handler_ = &default_event_handler_;
    } else {
        event_handler_ = event_handler;
    }
}

Connection::~Connection(void)
{
    release();
}

int32_t Connection::initialize(void)
{
    int32_t result = E_ERROR;
    int32_t retcode = E_ERROR;

    if (init_ok_) {
        goto ExitOK;
    }

    // get a new valid sockfd and prepare to poll it
    ASSURE_SUCCESS(result, retcode, sockfd_.initialize());
    init_ok_ = S_SUCCESS;
    set_state(CLOSED);
    event_.data.ptr = this;
    set_events();
    recv_buf_.clear();
    send_buf_.clear();

    if (NULL != connection_manager_) {
        ASSURE_SUCCESS(result, retcode, connection_manager_->controlAdd(this));
    }

ExitOK:
    result = S_SUCCESS;
ExitError:
    if (S_SUCCESS != result) {
        release();
    }

    return result;
}

int32_t Connection::initialize(in_addr_t sockfd,
    struct sockaddr_in sain)
{
    int32_t result = E_ERROR;
    int32_t retcode = E_ERROR;

    if (init_ok_) {
        goto ExitOK;
    }

    // get a new valid sockfd and prepare to poll it
    ASSURE_SUCCESS(result, retcode, sockfd_.initialize(sockfd));
    sain_ = sain;
    init_ok_ = S_SUCCESS;
    set_state(ESTABLISHED);
    event_.data.ptr = this;
    set_events();
    recv_buf_.clear();
    send_buf_.clear();

    if (NULL != connection_manager_) {
        ASSURE_SUCCESS(result, retcode, connection_manager_->controlAdd(this));
    }

ExitOK:
    result = S_SUCCESS;
ExitError:
    if (S_SUCCESS != result) {
        release();
    }

    return result;
}

int32_t Connection::recv(void)
{
    int32_t result = E_ERROR;
    int8_t buffer[BUF_SIZE];
    int32_t bytes = 0;

    if (recv_buf_.freeSize() < bytes) {
        goto ExitError;
    }

RE_RECV:
    if (ESTABLISHED != state()) {
        result = E_NOT_CONN;
        goto ExitError;
    }

    if (-1 == (bytes = ::recv(sockfd_, buffer, BUF_SIZE, MSG_DONTWAIT))) {
        perror("recv");
        bytes = 0;
        if (EINTR == errno) {
            goto RE_RECV;
        } else if (EWOULDBLOCK != errno) {
            set_state(CLOSE_WAIT);
            result = E_ERROR;
            goto ExitError;
        }
    } else if (0 == bytes) {
        set_state(CLOSE_WAIT);
        result = E_ERROR;
        goto ExitError;
    }
    recv_buf_.write(buffer, bytes);

    result = S_SUCCESS;
ExitError:
    return result;
}

int32_t Connection::send(void)
{
    int32_t result = E_ERROR;
    int8_t buffer[BUF_SIZE];
    int32_t bytes = 0;

    if ((bytes = send_buf_.peek(buffer, BUF_SIZE)) <= 0) {
        goto ExitOK;
    }
RE_SEND:
    if (ESTABLISHED != state()) {
        result = E_NOT_CONN;
        goto ExitError;
    }
    if (-1 == (bytes = ::send(sockfd_, buffer, bytes, MSG_DONTWAIT))) {
        perror("send");
        bytes = 0;
        if (EINTR == errno) {
            goto RE_SEND;
        } else if (EWOULDBLOCK != errno) {
            set_state(CLOSE_WAIT);
            result = E_ERROR;
            goto ExitError;
        }
    } else if (0 == bytes) {
        set_state(CLOSE_WAIT);
        result = E_ERROR;
        goto ExitError;
    }
    send_buf_.seek(bytes);

ExitOK:
    result = S_SUCCESS;
ExitError:
    return result;
}

int32_t Connection::release(void)
{
    int32_t result = E_ERROR;

    if (!init_ok_) {
        goto ExitOK;
    }

    event_handler_->onRelease(this);
    if (NULL != connection_manager_) {
        connection_manager_->controlDel(this);
    }
    sockfd_.release();
    init_ok_ = E_ERROR;
    set_state(CLOSED);

ExitOK:
    result = S_SUCCESS;
    return result;
}

int32_t Connection::listen(void)
{
    int32_t result = E_ERROR;
    int32_t retcode = E_ERROR;

    if (LISTEN == state()) {
        goto ExitOK;
    } else if (CLOSED != state()) {
        result = E_SYS_ERROR_STATE;
        goto ExitError;
    }

    ASSURE_SUCCESS(result, retcode, initialize());
    if (-1 == (retcode = ::bind(sockfd_.sockfd(), sain_, sain_.len()))) {
        perror("bind");
        result = retcode;
        goto ExitError;
    }
    if (-1 == (retcode = ::listen(sockfd_.sockfd(), 1024))) {
        perror("listen");
        result = retcode;
        goto ExitError;
    }

    set_state(LISTEN);
ExitOK:
    result = S_SUCCESS;
ExitError:
    if (S_SUCCESS != result) {
        event_handler_->onListenFailed(this);
        release();
    }
    return result;
}

int32_t Connection::accept(void)
{
    int32_t result = E_ERROR;
    int32_t retcode = E_ERROR;
    int32_t connfd = 0;
    struct sockaddr_in sain;
    socklen_t socklen = sizeof(sain);
    Connection* conn = NULL;

    if (LISTEN != state()) {
        result = E_SYS_ERROR_STATE;
        goto ExitError;
    }
    if (-1 == (connfd = ::accept(sockfd_.sockfd(), (SockaddrIn::PSA)&sain, &socklen))) {
        perror("accept");
        result = retcode;
        goto ExitError;
    }
    ASSURE_SUCCESS(result, retcode, connection_manager_->createConnection(conn));
    ASSURE_SUCCESS(result, retcode, conn->initialize(connfd, sain));
    conn->set_event_handler(event_handler_);
    event_handler_->onAccept(conn);
    event_handler_->onConnectEstablished(conn);

    result = S_SUCCESS;
ExitError:
    if (S_SUCCESS != result) {
        close(connfd);
        connfd = 0;
        event_handler_->onAcceptFailed(this);
    }
    return result;
}

int32_t Connection::connect(void)
{
    int32_t result = E_ERROR;
    int32_t retcode = E_ERROR;

    if (CLOSED != state()) {
        result = E_SYS_ERROR_STATE;
        goto ExitError;
    }

    ASSURE_SUCCESS(result, retcode, initialize());
    if (-1 == (retcode = ::connect(sockfd_.sockfd(), sain_, sain_.len()))) {
        if (errno != EINPROGRESS) {
            perror("connect");
            result = retcode;
            goto ExitError;
        }
        set_state(CONNECT_SENT);
    } else if (0 == retcode) { // connection established immediately
        set_state(ESTABLISHED);
        event_handler_->onConnectEstablished(this);
    }

    result = S_SUCCESS;
ExitError:
    if (S_SUCCESS != result) {
        event_handler_->onConnectFailed(this);
        release();
    }
    return result;
}

int32_t Connection::disconnect(void)
{
    if (CLOSED != state()) {
        release();
    }
    return S_SUCCESS;
}

int32_t Connection::read(void* buffer, 
    int32_t want_read_bytes, 
    int32_t& real_read_bytes) 
{
    int32_t result = E_ERROR;
    int32_t bytes = 0;

    if (ESTABLISHED != state() && (recv_buf_.dataSize() <= 0)) {
        result = E_NOT_CONN;
        goto ExitError;
    }

    if ((bytes = recv_buf_.read(buffer, want_read_bytes)) < 0) {
        bytes = 0;
    }

    result = S_SUCCESS;
ExitError:
    real_read_bytes = bytes;
    return result;
}

int32_t Connection::peek(void* buffer, 
    int32_t want_read_bytes, 
    int32_t& real_read_bytes) 
{
    int32_t result = E_ERROR;
    int32_t bytes = 0;

    if (ESTABLISHED != state() && (recv_buf_.dataSize() <= 0)) {
        result = E_NOT_CONN;
        goto ExitError;
    }

    if ((bytes = recv_buf_.peek(buffer, want_read_bytes)) < 0) {
        bytes = 0;
    }

    result = S_SUCCESS;
ExitError:
    real_read_bytes = bytes;
    return result;
}

int32_t Connection::seek(int32_t bytes)
{
    recv_buf_.seek(bytes);
    return S_SUCCESS;
}

int32_t Connection::peekUInt16(uint16_t& val)
{
    return recv_buf_.peekUInt16(val);
}

int32_t Connection::write(const int8_t* buffer,
    int32_t want_write_bytes,
    int32_t& real_write_bytes)
{
    int32_t result = E_ERROR;
    int32_t bytes = 0;

    if (ESTABLISHED != state()) {
        result = E_NOT_CONN;
        goto ExitError;
    }

    if ((bytes = send_buf_.write(buffer, want_write_bytes)) < 0) {
        bytes = 0;
    }

    result = S_SUCCESS;
ExitError:
    real_write_bytes = bytes;
    return result;
}

int32_t Connection::onEvents(const Event& event)
{
    int32_t result = E_ERROR;
    int32_t retcode = E_ERROR;
    uint32_t events = event.events;
#ifdef DEBUG_TRACE/*{{{*/
    std::cout << "#[Connection::OnEvents] sockfd["
        << sockfd_.sockfd()
        << "], state["
        << state()
        << "]"
        << std::endl;
#endif // DEBUG_TRACE/*}}}*/
    if (LISTEN == state()) {
        if (EPOLLIN & events) {/*{{{*/
            if (S_SUCCESS != (retcode = accept())) {
                event_handler_->onAcceptFailed(this);
                result = retcode;
                goto ExitError;
            }
        } else {
            event_handler_->onError(this);
            disconnect();
            goto ExitError;
        }/*}}}*/
    } else if (CONNECT_SENT == state()) {
        if (!sockfd_.isError() && ((EPOLLIN & events) || (EPOLLOUT & events))) {/*{{{*/
            set_state(ESTABLISHED);
            event_handler_->onConnectEstablished(this);
        } else {
            event_handler_->onConnectFailed(this);
            disconnect();
            goto ExitError;
        }/*}}}*/
    } else if (ESTABLISHED == state()) {
        if (EPOLLIN & events) {/*{{{*/
            recv();
            retcode = event_handler_->onRead(this);
        }
        if (EPOLLOUT & events) {
            retcode = event_handler_->onWrite(this);
            send();
        }
        if (EPOLLPRI & events) {
            retcode = event_handler_->onPrimaryData(this);
        }
        if (EPOLLERR & events) {
            retcode = event_handler_->onError(this);
            disconnect();
            goto ExitError;
        }
        if (EPOLLHUP & events) {
            retcode = event_handler_->onHup(this);
            disconnect();
            goto ExitError;
        }/*}}}*/
    } else {
        goto ExitOK;
    }

ExitOK:
    result = S_SUCCESS;
ExitError:
    if (CLOSE_WAIT == state()) {
        event_handler_->onClose(this);
        disconnect();
    }
    return result;
}

const char* Connection::string_ip(void) const
{
    static char ip_string[INET6_ADDRSTRLEN] = {0};

    sain_.ip(ip_string, sizeof(ip_string));

    return ip_string;
}

int32_t Connection::set_ip(const char* string_ip)
{
    return sain_.set_ip(string_ip);
}

uint32_t Connection::numeric_ip(void) const
{
    return sain_.ip();
}

int32_t Connection::set_ip(uint32_t numeric_ip)
{
    return sain_.set_ip(numeric_ip);
}

uint16_t Connection::port(void) const
{
    return sain_.port();
}

int32_t Connection::set_port(uint16_t port)
{
    return sain_.set_port(port);
}

const char* Connection::protocol(void) const
{
    const char* tcp_protocol = NULL;
    const char* tcp6_protocol = NULL;
    const char* udp_protocol = NULL;
    const char* udp6_protocol = NULL;

    tcp_protocol = "tcp";
    tcp6_protocol = "tcp6";
    udp_protocol = "udp";
    udp6_protocol = "udp6";

    if (sockfd_.type() == SOCK_DGRAM) {
        if (sockfd_.family() == AF_INET) {
            return udp_protocol;
        } else if (sockfd_.family() == AF_INET6) {
            return udp6_protocol;
        } else {
            return NULL;
        }
    } else if (sockfd_.type() == SOCK_STREAM) {
        if (sockfd_.family() == AF_INET) {
            return tcp_protocol;
        } else if (sockfd_.family() == AF_INET6) {
            return tcp6_protocol;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }

}

int32_t Connection::set_protocol(const char* protocol_string)
{
    sa_family_t family = 0;
    int32_t sock_type = 0;
    int32_t protocol = 0;

    if (strcmp("udp", protocol_string) == 0) {
        family = AF_INET;
        sock_type = SOCK_DGRAM;
        protocol = 0;
    } else if (strcmp("udp6", protocol_string) == 0) {
        family = AF_INET6;
        sock_type = SOCK_DGRAM;
        protocol = 0;
    } else if (strcmp("tcp", protocol_string) == 0) {
        family = AF_INET;
        sock_type = SOCK_STREAM;
        protocol = 0;
    } else if (strcmp("tcp6", protocol_string) == 0) {
        family = AF_INET6;
        sock_type = SOCK_STREAM;
        protocol = 0;
    } else {
        return E_ERROR;
    }

    sockfd_.set_family(family);
    sockfd_.set_type(sock_type);
    sockfd_.set_protocol(protocol);
    sain_.set_family(family);

    return S_SUCCESS;
}

Sockfd& Connection::sockfd(void)
{
    return sockfd_;
}

const Sockfd& Connection::sockfd(void) const
{
    return sockfd_;
}

uint32_t Connection::events(void) const
{
    return event_.events;
}

int32_t Connection::set_events(uint32_t events)
{
    event_.events = events;
    return S_SUCCESS;
}

struct epoll_event Connection::epollevent(void) const
{
    return event_;
}

struct epoll_event* Connection::pepollevent(void)
{
    return &event_;
}

EventHandler* Connection::event_handler(void) const
{
    return event_handler_;
}

int32_t Connection::set_event_handler(EventHandler* event_handler)
{
    event_handler_ = event_handler;
    return S_SUCCESS;
}

Connection::State Connection::state(void) const
{
    return state_;
}

int32_t Connection::set_state(State state)
{
    state_ = state;
    return S_SUCCESS;
}

bool Connection::auto_release(void) const
{
    return auto_release_;
}

int32_t Connection::set_auto_release(bool auto_release)
{
    auto_release_ = auto_release;
    return S_SUCCESS;
}

ConnectionManager* Connection::connection_manager(void)
{
    return connection_manager_;
}

int32_t Connection::set_connection_manager(ConnectionManager* const connection_manager)
{
    connection_manager_ = connection_manager;
    return S_SUCCESS;
}

} // end of namespace socketor

