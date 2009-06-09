#include "sockfd.h"

#include <errno.h>      // for errno
#include <unistd.h>
#include "stdio.h"

namespace socketor {
// constructors
Sockfd::Sockfd(void) :
    family_(AF_INET),
    type_(SOCK_STREAM),
    protocol_(IPPROTO_TCP),
    sockfd_(0)
{
}

// destructors
Sockfd::~Sockfd(void)
{
    release();
}

// getter and setter
sa_family_t Sockfd::family(void) const
{
    return family_;
}

int32_t Sockfd::set_family(sa_family_t new_family)
{
    if (AF_INET == new_family || 
        AF_INET6 == new_family ||
        AF_LOCAL == new_family || 
        AF_ROUTE == new_family ||
        AF_KEY == new_family) {
        family_ = new_family;
        return S_SUCCESS;
    } else {
        return E_PARAMETER_ERROR;
    }
}

int32_t Sockfd::type(void) const
{
    return type_;
}

int32_t Sockfd::set_type(int32_t new_type)
{
    if (SOCK_STREAM == new_type ||
        SOCK_DGRAM == new_type ||
        SOCK_SEQPACKET == new_type ||
        SOCK_RAW == new_type) {
        type_ = static_cast<__socket_type>(new_type);
        return S_SUCCESS;
    } else {
        return E_PARAMETER_ERROR;
    }
}

int32_t Sockfd::protocol(void) const
{
    return protocol_;
}

int32_t Sockfd::set_protocol(int32_t new_protocol)
{
    if (IPPROTO_IP == new_protocol ||
        IPPROTO_TCP == new_protocol ||
        IPPROTO_UDP == new_protocol ||
        IPPROTO_SCTP == new_protocol) {
        protocol_ = new_protocol;
        return S_SUCCESS;
    } else {
        return E_PARAMETER_ERROR;
    }
}

in_addr_t Sockfd::sockfd(void) const
{
    return sockfd_;
}

// operators
int32_t Sockfd::initialize(void)
{
    int32_t sockfd = 0;

    release();
    sockfd = socket(family_, type_, protocol_);

    if (sockfd > 0) {
        sockfd_ = sockfd;
        if ((0 == setReuseAddr()) &&
            // (0 == setLinger()) &&
            (0 == setNonBlocking())) {
            return S_SUCCESS;
        } else {
            sockfd_ = 0;
            return E_ERROR;
        }
    } else {
        return E_ERROR;
    }
}

int32_t Sockfd::initialize(in_addr_t sockfd)
{
    release();
    if (sockfd > 2) {
        sockfd_ = sockfd;
        if ((0 == setReuseAddr()) &&
            // (0 == setLinger()) &&
            (0 == setNonBlocking())) {
            return S_SUCCESS;
        } else {
            sockfd_ = 0;
            return E_ERROR;
        }
    } else {
        return E_ERROR;
    }
}

int32_t Sockfd::release(void)
{
    // is this needed?
    if (sockfd() < 3) {
        return S_SUCCESS;
    }

    close(sockfd());
    sockfd_ = 0;

    return S_SUCCESS;
}

int32_t Sockfd::isError(void)
{
    int32_t retcode = E_ERROR;
    int32_t error = E_ERROR;
    socklen_t len = E_ERROR;

    len = sizeof(error);
    if ((-1 == (retcode = getsockopt(sockfd(), SOL_SOCKET, SO_ERROR, &error, &len))) ||
        (error != 0)) {
        return 1;
    } else {
        return 0;
    }
}

int32_t Sockfd::setNonBlocking(void)
{
    return fcntl(sockfd_, F_SETFL, 
        fcntl(sockfd_, F_GETFL, 0) | O_NONBLOCK);
}

int Sockfd::setReuseAddr(void)
{
    int on = 1;
    if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &on,
            sizeof(on)) == -1) {
        return -1;
    }
    return 0;
}

int Sockfd::setLinger(int on, int to)
{
    struct linger so_linger = {on, to};

    if (setsockopt(sockfd_, SOL_SOCKET, SO_LINGER, &so_linger,
            sizeof(so_linger)) == -1) {
        return -1;
    }
    return 0;
}

long Sockfd::getSockMaxBuf(void)
{
    return fpathconf(sockfd_, _PC_SOCK_MAXBUF);
}

int32_t Sockfd::getSendBufSize(void)
{
    int32_t size = 0;
    socklen_t len = sizeof(size);
    if (-1 == getsockopt(sockfd_, SOL_SOCKET, SO_SNDBUF, &size, &len)) {
        return -1;
    } else {
        return size;
    }
}

int32_t Sockfd::getRecvBufSize(void)
{
    int32_t size = 0;
    socklen_t len = sizeof(size);
    if (-1 == getsockopt(sockfd_, SOL_SOCKET, SO_RCVBUF, &size, &len)) {
        return -1;
    } else {
        return size;
    }
}

int32_t Sockfd::setSendBufSize(int32_t size)
{
    socklen_t len = sizeof(size);
    return setsockopt(sockfd_, SOL_SOCKET, SO_SNDBUF, &size, len);
}

int32_t Sockfd::setRecvBufSize(int32_t size)
{
    socklen_t len = sizeof(size);
    return setsockopt(sockfd_, SOL_SOCKET, SO_RCVBUF, &size, len);
}

Sockfd::operator int32_t(void) const
{
    return static_cast<int32_t>(sockfd());
}

Sockfd::operator uint32_t(void) const
{
    return static_cast<uint32_t>(sockfd());
}
} // end of namespace socketor

