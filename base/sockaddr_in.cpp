#include "sockaddr_in.h"

#include <string.h>
#include <iostream>

#include <netinet/in.h>     // for INET_ADDRSTRLEN(16) and INET6_ADDRSTRLEN(46)
#include <arpa/inet.h>      // for inet_ntop, inet_pton
#include <errno.h>

using namespace std;

namespace socketor {
// constructors
SockaddrIn::SockaddrIn(void)
{
    initinalize();
}

SockaddrIn::SockaddrIn(const struct sockaddr_in& sain)
{
    sain_ = sain;
}

SockaddrIn::SockaddrIn(const SockaddrIn& rhs)
{
    sain_ = rhs.sain_;
}

SockaddrIn& SockaddrIn::operator = (const SockaddrIn& rhs)
{
    if (this == &rhs) {
        goto ExitOK;
    }

    sain_ = rhs.sain_;

ExitOK:
    return *this;
}

// destructor
SockaddrIn::~SockaddrIn(void)
{
}

// operators
sa_family_t SockaddrIn::family(void) const
{
    return sain_.sin_family;
}

int32_t SockaddrIn::set_family(sa_family_t family)
{
    if (AF_INET == family || 
        AF_INET6 == family ||
        AF_LOCAL == family || 
        AF_ROUTE == family ||
        AF_KEY == family) {
        sain_.sin_family = family;
        return S_SUCCESS;
    } else {
        return E_PARAMETER_ERROR;
    }
}

in_addr_t SockaddrIn::ip(void* addrptr) const
{
    in_addr_t host_byte_order_addr = ntohl(sain_.sin_addr.s_addr);

    if (NULL != addrptr) {
        memcpy(addrptr, &host_byte_order_addr, sizeof(host_byte_order_addr));
    }

    return host_byte_order_addr;
}

int32_t SockaddrIn::set_ip(in_addr_t addr)
{
    sain_.sin_addr.s_addr = htonl(addr);
    return S_SUCCESS;
}

const char* SockaddrIn::ip(char* strptr, size_t len) const
{
    return inet_ntop(family(), &sain_.sin_addr, strptr, len);
}

int32_t SockaddrIn::set_ip(const char* strptr)
{
    int32_t ret = 0;

    ret = inet_pton(family(), strptr, &sain_.sin_addr);

    if (1 == ret) {
        return S_SUCCESS;
    } else if (0 == ret) {
        return -312/*E_SYS_NET_WRONG_STR_IP_FORMAT*/;
    } else {
        return errno;
    }
}

in_port_t SockaddrIn::port(void* portptr) const
{
    in_port_t host_byte_order_port = ntohs(sain_.sin_port);

    if (NULL != portptr) {
        memcpy(portptr, &host_byte_order_port, sizeof(host_byte_order_port));
    }

    return host_byte_order_port;
}

int32_t SockaddrIn::set_port(in_port_t port)
{
    sain_.sin_port = htons(port);
    return S_SUCCESS;
}

socklen_t SockaddrIn::len(void) const
{
    return sizeof(SAIN);
}

// type cast
SockaddrIn::operator PSA(void)
{
    return reinterpret_cast<PSA>(&sain_);
}

SockaddrIn::operator const struct sockaddr*(void) const
{
    return reinterpret_cast<const struct sockaddr* const>(&sain_);
}

// methods
int32_t SockaddrIn::initinalize(void)
{
    sain_.sin_family = DEF_FAMILY,
    sain_.sin_addr.s_addr = DEF_ADDR,
    sain_.sin_port = DEF_PORT;

    return S_SUCCESS;
}
} // end of namespace socketor

