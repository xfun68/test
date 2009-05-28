#include <iostream>

#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>     // in_addr_t, IPPROTO_TCP ...

using namespace std;

int main(int argn, char* argv[])
{
    int rc = 0;

    for (int i = 0; i < 2048; ++i)
    {
        rc = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

        if (rc == -1) {
            cout << "errno = " << errno << endl;
            perror("errno");
            break;
        }

        cout << i << " sockfd = " << rc << endl;
    }

    return 0;
}
