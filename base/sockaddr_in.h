#ifndef _SOCKADDR_H_
#define _SOCKADDR_H_

#include "OperationCode.h"

#include <sys/types.h>      // for sa_family_t, sa_port_t, int32_t ...
#include <sys/socket.h>     // for socklen_t 
#include <netinet/in.h>     // for INET_ADDRSTRLEN(16) and
// INET6_ADDRSTRLEN(46), struct sockaddr_in

namespace socketor {
    class SockaddrIn
    {
    public:
        typedef struct sockaddr SA;
        typedef struct sockaddr* PSA;
        typedef struct sockaddr_in SAIN;
        typedef struct sockaddr_in* PSAIN;

        enum {
            DEF_PORT = 0u,
            DEF_ADDR = 2130706433U, // "127.0.0.1"
            DEF_FAMILY = AF_INET
        };

    public:
        // constructors
        SockaddrIn(void);
        SockaddrIn(const struct sockaddr_in& sain);
        SockaddrIn(const SockaddrIn& rhs);
        SockaddrIn& operator = (const SockaddrIn& rhs);

        // destructor
        ~SockaddrIn(void);

        // operators
        sa_family_t family(void) const;
        int32_t set_family(sa_family_t family = DEF_FAMILY);

        in_addr_t ip(void* addrptr = NULL) const;
        int32_t set_ip(in_addr_t addr);
        const char* ip(char* strptr, size_t len) const;
        int32_t set_ip(const char* strptr);

        in_port_t port(void* portptr = NULL) const;
        int32_t set_port(in_port_t port);

        socklen_t len(void) const;

        // tepe cast
        operator PSA(void); 
        operator const struct sockaddr*(void) const; 

    private:
        // methods
        int32_t initinalize(void);

    private:
        SAIN sain_;
    };
} // end of namespace socketor
#endif // _SOCKADDR_H_

