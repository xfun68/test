/*  
 *  ����Ĭ�Ϲ������tcp���͵�socket����������ʱ��֧��udp���������͵�
 *    socket��������
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
         * ���ܣ�   ��ȡ��ǰϵͳ������׽�����󻺳�����С
         * ���أ�
         *  # -1    ʧ��
         *  # >0    �������Ĵ�С
         */
        long getSockMaxBuf(void);
        /*
         * ���ܣ�   ��ȡ�׽��ֵ�ǰ�ķ��ͻ�������С
         * ���أ�
         *  # >0    �������Ĵ�С
         *  # -1    ʧ��
         */
        int32_t getSendBufSize(void);
        /*
         * ���ܣ�   ��ȡ�׽��ֵ�ǰ�Ľ��ջ�������С
         * ���أ�
         *  # >0    �������Ĵ�С
         *  # -1    ʧ��
         */
        int32_t getRecvBufSize(void);
        /*
         * ���ܣ�   �����׽��ֵ�ǰ�ķ��ͻ�������С
         * ���أ�
         *  # 0     �ɹ�
         *  # -1    ʧ��
         */
        int32_t setSendBufSize(int32_t size);
        /*
         * ���ܣ�   �����׽��ֵ�ǰ�Ľ��ջ�������С
         * ���أ�
         *  # 0     �ɹ�
         *  # -1    ʧ��
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

