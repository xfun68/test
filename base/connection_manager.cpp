#include "connection_manager.h"

#include <errno.h>
#include <sys/time.h>

#include <iostream>
#include <algorithm>

using namespace std;

namespace socketor
{

ConnectionManager::ConnectionManager(EventHandler* event_handler) :
    epfd_(0),
    init_ok_(0),
    default_event_handler_("ConnMgrDefHandler"),
    event_q_(NULL)
{
    if (NULL != event_handler_) {
        event_handler_ = event_handler;
    } else {
        event_handler_ = &default_event_handler_;
    }
}

ConnectionManager::~ConnectionManager(void)
{
    release();
}

int32_t ConnectionManager::initialize(void)
{
    int32_t result = E_ERROR;
    int32_t retcode = E_ERROR;

    if (init_ok_) {
        goto ExitOK;
    }

    if (-1 == (retcode = epoll_create(MAX_CONNECTION_NUM))) {
        result = retcode;
        goto ExitError;
    }
    try {
        event_q_ = new struct epoll_event[MAX_CONNECTION_NUM];
    } catch (bad_alloc ba) {
        cout << ba.what() << endl;
        result = E_ERROR;
        goto ExitError;
    }
    initializeConnectionList();

    epfd_ = retcode;
    init_ok_ = S_SUCCESS;
ExitOK:
    result = S_SUCCESS;
ExitError:
    if (S_SUCCESS != result) {
        release();
    }
    return result;
}

int32_t ConnectionManager::release(void)
{
    int32_t result = E_ERROR;

    if (!init_ok_) {
        goto ExitOK;
    }

    releaseConnectionList();
    close(epfd_);
    epfd_ = 0;
    init_ok_ = E_ERROR;

ExitOK:
    result = S_SUCCESS;
    return result;
}

int32_t ConnectionManager::initializeConnectionList(void)
{
    conn_list_.clear();
    return S_SUCCESS;
}

int32_t ConnectionManager::releaseConnectionList(void)
{
    int32_t conn_numbers = 0;
    for (ConnectionListItr itr(conn_list_.begin());
        itr != conn_list_.end();
        ++itr) {
        (*itr)->release();
        delete *itr;
        *itr = NULL;
        ++conn_numbers;
    }
    conn_list_.clear();
#ifdef DEBUG_TRACE // {{{
    cout << "#[ConnMgr::ReleaseConnectionList] "
        << "released "
        << conn_numbers
        << " connection(s)"
        << endl;
#endif // DEBUG_TRACE }}}
    return S_SUCCESS;
}

int32_t ConnectionManager::getConnctionNumber(void)
{
    return static_cast<int32_t>(conn_list_.size());
}

int32_t ConnectionManager::createConnection(uint32_t ip,
    uint16_t port,
    Connection*& conn)
{
    int32_t result = E_ERROR;

    conn = NULL;
    try {
        conn = new Connection(ip, port);
    } catch (bad_alloc) {
        result = E_ERROR;
        goto ExitError;
    }
    conn->set_connection_manager(this);
    conn_list_.push_back(conn);

    result = S_SUCCESS;
ExitError:

    return result;
}

int32_t ConnectionManager::createConnection(const char* ip,
    uint16_t port,
    Connection*& conn)
{
    int32_t result = E_ERROR;

    conn = NULL;
    try {
        conn = new Connection(ip, port);
    } catch (bad_alloc) {
        result = E_ERROR;
        goto ExitError;
    }
    conn->set_connection_manager(this);
    conn_list_.push_back(conn);

    result = S_SUCCESS;
ExitError:
    return result;
}

int32_t ConnectionManager::createConnection(Connection*& conn)
{
    int32_t result = E_ERROR;

    conn = NULL;
    try {
        conn = new Connection();
    } catch (bad_alloc) {
        result = E_ERROR;
        goto ExitError;
    }
    conn->set_connection_manager(this);
    conn_list_.push_back(conn);

    result = S_SUCCESS;
ExitError:
    return result;
}

int32_t ConnectionManager::createPollessConnection(Connection*& conn)
{
    int32_t result = E_ERROR;

    conn = NULL;
    try {
        conn = new Connection();
    } catch (bad_alloc) {
        result = E_ERROR;
        goto ExitError;
    }
    conn->set_connection_manager(NULL);
    conn_list_.push_back(conn);

    result = S_SUCCESS;
ExitError:
    return result;
}

int32_t ConnectionManager::releaseConnection(Connection* conn)
{
    int32_t result = E_ERROR;

    ConnectionListItr itr = NULL;
    itr = find(conn_list_.begin(), conn_list_.end(), conn);
    if (itr == conn_list_.end()) {
        result = E_ERROR;
        goto ExitError;
    }

    conn->release();
    delete conn;
    conn_list_.erase(itr);

    result = S_SUCCESS;
ExitError:
    return result;
}

int32_t ConnectionManager::controlAdd(Connection* const conn)
{
    int32_t result = E_ERROR;
    int32_t retcode = E_ERROR;

    if (-1 == (retcode = epoll_ctl(epfd_, EPOLL_CTL_ADD,
                conn->sockfd(), conn->pepollevent()))) {
        perror("ControlAdd");
#ifdef DEBUG_TRACE // {{{
        cout << "#[ConnMgr::ControlAdd] epoll_ctl add fd["
            << conn->sockfd().sockfd()
            << "] failed, errno["
            << errno
            << "]"
            << endl;
#endif // DEBUG_TRACE }}}
        result = retcode;
        goto ExitError;
    }
#ifdef DEBUG_TRACE // {{{
    cout << "#[ConnMgr::ControlAdd] epoll_ctl add fd["
        << conn->sockfd().sockfd()
        << "] OK"
        << endl;
#endif // DEBUG_TRACE }}}
    result = S_SUCCESS;
ExitError:
    return result;
}

int32_t ConnectionManager::controlDel(Connection* conn)
{
    int32_t result = E_ERROR;
    int32_t retcode = E_ERROR;

    if (-1 == (retcode = epoll_ctl(epfd_, EPOLL_CTL_DEL,
                conn->sockfd(), conn->pepollevent()))) {
        perror("ControlDel");
#ifdef DEBUG_TRACE // {{{
        cout << "#[ConnMgr::ControlDel] epoll_ctl delete fd["
            << conn->sockfd().sockfd()
            << "] failed, errno["
            << errno
            << "]"
            << endl;
#endif // DEBUG_TRACE }}}
        result = retcode;
        goto ExitError;
    }
#ifdef DEBUG_TRACE // {{{
    cout << "#[ConnMgr::ControlDel] epoll_ctl delete fd["
        << conn->sockfd().sockfd()
        << "] OK"
        << endl;
#endif // DEBUG_TRACE }}}
    result = S_SUCCESS;
ExitError:
    return result;
}

int32_t ConnectionManager::run(void)
{
    int32_t retcode = E_ERROR;
    int32_t nfds = 0;
#ifdef DEBUG_TRACE // {{{
    int32_t i = 0;
    struct timeval old_time;
    struct timeval curr_time;
#endif // DEBUG_TRACE }}}
    Connection* conn = NULL;
    while (init_ok_) {
#ifdef DEBUG_TRACE // {{{
        ++i;
        gettimeofday(&old_time, NULL);
#endif // DEBUG_TRACE }}}
        event_handler_->onBeginLoop(this);
        nfds = epoll_wait(epfd_, event_q_, MAX_CONNECTION_NUM, 0);
        if (0 == nfds) {
            event_handler_->onIdle(this);
        } else {
            if (nfds > 0) {
#ifdef DEBUG_TRACE // {{{
                cout << "#[ConnMgr::Loop] there are "
                    << nfds
                    << " connection(s) have events toprocess in "
                    << i
                    << "th loop "
                    << endl;
#endif // DEBUG_TRACE }}}
                for(int32_t n = 0; n < nfds; ++n) {
                    conn = static_cast<Connection*>(event_q_[n].data.ptr);
                    retcode = conn->onEvents(event_q_[n]);
                    if ((Connection::CLOSED == conn->state()) &&
                        (conn->auto_release())) {
                        releaseConnection(conn);
                    }
                }
#ifdef DEBUG_TRACE // {{{
                cout << "#[ConnMgr::Loop] There are "
                    << nfds
                    << " connection(s) have events processed in "
                    << i
                    << "th loop "
                    << endl;
#endif // DEBUG_TRACE }}}
            } else {
                perror("epoll_wait");
#ifdef DEBUG_TRACE // {{{
                cout << "#[ConnMgr::Loop] epoll_wait failed, errno["
                    << errno
                    << "]"
                    << endl;
#endif // DEBUG_TRACE }}}
            }
        }
        event_handler_->onEndLoop(this);
#ifdef DEBUG_TRACE // {{{
        gettimeofday(&curr_time, NULL);

        // 如果此次循环耗时大于等于1秒，则直接进入下一次循环  
        if ((curr_time.tv_sec - old_time.tv_sec) < 1) {
            curr_time.tv_usec = USEC_PER_SEC - curr_time.tv_usec;
        } else {
            curr_time.tv_usec = 0;
        }
        curr_time.tv_sec = 0;
        cout << "#[ConnMgr::Loop] sleep ";
        cout.width(6);
        cout.fill('0');
        cout << right << curr_time.tv_usec
            << " us, "
            << i
            << "th loop "
            << endl;
        usleep(curr_time.tv_usec);
#else
        usleep(10*1000);
#endif // DEBUG_TRACE // }}}
    }

    return S_SUCCESS;
}

int32_t ConnectionManager::setEventHandler(EventHandler* event_handler)
{
    if (NULL == event_handler) {
        return E_SYS_INVALID_HANDLER;
    } else {
        event_handler_ = event_handler;
        return S_SUCCESS;
    }
}

int32_t ConnectionManager::getEventHandler(EventHandler*& event_handler)
{
    event_handler = event_handler_;
    return S_SUCCESS;
}

} // end of namespace socketor

