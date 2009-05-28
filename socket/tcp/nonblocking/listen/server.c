#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>


#define MAXBUF 1024
#define MAXEPOLLSIZE 10000

/*
 * setreuseaddr - ����Ϊ����������ģʽ
 */
int setreuseaddr(int sockfd)
{
    int on = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
        perror("setreuseaddr");
        return -1;
    }
    return 0;
}

/*
 * setnonblocking - ���þ��Ϊ��������ʽ
 */
int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) {
        perror("setnonblocking");
        return -1;
    }
    return 0;
}

/*
 * setlinger - ����close�ľ�����Ϊ
 */
int setlinger(int sockfd, int on, int to)
{
    struct linger so_linger = {on, to};

    if (setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger)) == -1) {
        perror("setlinger");
        return -1;
    }
    return 0;
}

/*
   handle_message - ����ÿ�� socket �ϵ���Ϣ�շ�
 */
int handle_message(int new_fd)
{
    char buf[MAXBUF + 1];
    int len;
    /* 
     * ��ʼ����ÿ���������ϵ������շ� 
     */
    bzero(buf, MAXBUF + 1);
    /* 
     * ���տͻ��˵���Ϣ 
     */
    len = recv(new_fd, buf, MAXBUF, 0);
    if (len > 0)
        printf ("socket[%d]������Ϣ�ɹ�:\n'%s'��\n��%d���ֽڵ�����\n", 
            new_fd, buf, len);
    else {
        if (len < 0) {
            printf ("��Ϣ����ʧ�ܣ����������%d��������Ϣ��'%s'\n", 
                errno, strerror(errno));
        } else {
            printf ("�ͻ��˹ر������ӣ����������%d��������Ϣ��'%s'\n",
                errno, strerror(errno));
        }

        close(new_fd);
        return -1;
    }
    /* ����ÿ���������ϵ������շ����� */
    return len;
}

int main(int argc, char **argv)
{
    int listener, new_fd, kdpfd, nfds, n, ret, curfds;
    socklen_t len;
    struct sockaddr_in my_addr, their_addr;
    unsigned int myport, lisnum;
    struct epoll_event ev;
    struct epoll_event events[MAXEPOLLSIZE];
    struct rlimit rt;

    // if (argv[1])
    //     myport = atoi(argv[1]);
    // else
        myport = 9877;

    // if (argv[2])
    //     lisnum = atoi(argv[2]);
    // else
        lisnum = 1024;

    /* ����ÿ����������򿪵�����ļ��� */
    rt.rlim_max = rt.rlim_cur = MAXEPOLLSIZE;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1) {
        perror("setrlimit");
        exit(1);
    }
    else printf("����ϵͳ��Դ�����ɹ���\n");

    /* ���� socket ���� */
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    } else
        printf("socket �����ɹ���\n");

    if (setnonblocking(listener) != 0) {
        exit(1);
    }

    if (setreuseaddr(listener) != 0) {
        exit(1);
    }

    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(myport);
    // if (argv[3]) {
    //    my_addr.sin_addr.s_addr = inet_addr(argv[3]);
    // } else
        my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listener, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    } else
        printf("IP ��ַ�Ͷ˿ڰ󶨳ɹ�\n");

    if (listen(listener, lisnum) == -1) {
        perror("listen");
        exit(1);
    } else
        printf("��������ɹ���\n");

    /* ���� epoll ������Ѽ��� socket ���뵽 epoll ������ */
    kdpfd = epoll_create(MAXEPOLLSIZE);
    len = sizeof(struct sockaddr_in);
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listener;
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, listener, &ev) < 0) {
        fprintf(stderr, "epoll set insertion error: fd=%d\n", listener);
        return -1;
    } else
        printf("���� socket[%d] ���� epoll �ɹ���\n", listener);
    curfds = 1;

    while (1) {
        /* �ȴ����¼����� */
        nfds = epoll_wait(kdpfd, events, curfds, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }
        /* ���������¼� */
        for (n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listener) {
                new_fd = accept(listener, (struct sockaddr *) &their_addr, &len);
                if (new_fd < 0) {
                    perror("accept");
                    continue;
                } else
                    printf("�����������ڣ� %s:%d�� ����� socket Ϊ:%d\n", 
                        inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), new_fd);

                setnonblocking(new_fd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = new_fd;
                if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, new_fd, &ev) < 0) {
                    fprintf(stderr, "�� socket[%d] ���� epoll ʧ�ܣ�%s\n",
                        new_fd, strerror(errno));
                    return -1;
                }
                curfds++;
            } else {
                ret = handle_message(events[n].data.fd);
                if (ret < 1 && errno != 11) {
                    epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd,
                        &ev);
                    curfds--;
                }
            }
        }
    }
    close(listener);
    return 0;
}

