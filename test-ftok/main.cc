// =====================================================================================
// 
//       Filename:  main.cc
// 
//    Description:  ²âÊÔ ftok º¯ÊýµÄ demo ³ÌÐò
// 
//        Version:  1.0
//        Created:  05/31/2009 10:56:49 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Wang Xiaofeng (xfun68), xfun68@gmail.com
//        Company:  Linekong
// 
// =====================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>

int main (int argc, char *argv[])
{
    key_t my_key;

    puts(getenv("PWD"));
    my_key = ftok(getenv("PWD"), 0x010);
    printf("key token is: %u\n", my_key);

    return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

