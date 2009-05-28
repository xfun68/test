#include <rpc/rpc.h> 
#include <rpcsvc/rusers.h>
#include <stdio.h>

/*
 * a program that calls the
 * rusers() service
 */

int main(int argc,char **argv)
{
    int num;
    if (argc != 2) {
           fprintf(stderr, "usage: %s hostname\n",
                  argv[0]);
              exit(1);
                 }

    /*
    if ((num = rnusers(argv[1])) < 0) {
           fprintf(stderr, "error: rusers\n");
              exit(1);
                }
     */

    fprintf(stderr, "%d users on %s\n", num, argv[1] );
    exit(0);
}

