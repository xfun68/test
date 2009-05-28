#define RMTPROGNUM (u_long)0x3fffffffL
#define RMTPROGVER (u_long)0x1
#define RMTPROCNUM (u_long)0x1

#include <stdio.h>
#include <rpc/rpc.h>

int main(int argc, char **argv)
{
    int inproc=100, outproc;
    enum clnt_stat rstat;

    /* Service request to host RPCSERVER_HOST */
    if (rstat = rpc_call("as400.somewhere.ibm.com", RMTPROGNUM,
            RMTPROGVER, RMTPROCNUM, xdr_int, (char
                *)&inproc,
            xdr_int, (char
                *)&outproc,
            "VISIBLE")
        != RPC_SUCCESS){
        fprintf(stderr,"rpc_call() failed\n");
        exit(1);
    }

    exit(0);
}

