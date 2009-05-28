#include <stdio.h>
// #include <utmp.h> 
#include <rpc/rpc.h>
#include <rpcsvc/rusers.h>

/* a program that calls the RUSERSPROG
 * RPC program
 */

int main(int argc, char **argv)

{
    unsigned long nusers;
    enum clnt_stat cs;
    if (argc != 2) {
        fprintf(stderr, "usage: rusers hostname\n");
        exit(1);
    }

    if( cs = rpc_call(argv[1], RUSERSPROG,
            RUSERSVERS, RUSERSPROC_NUM, xdr_void,
            (char *)0, xdr_u_long, (char
                *)&nusers,
            "visible") !=
        RPC_SUCCESS ) {
        clnt_perrno(cs);
        exit(1);
    }
    /*
    */

    fprintf(stderr, "%d users on %s\n", nusers, argv[1] );
    exit(0);
}

