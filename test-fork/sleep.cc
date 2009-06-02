// =====================================================================================
// 
//       Filename:  sleep.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/02/2009 10:39:04 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Will King (xfun), xfun68@foxmail.com
//        Company:  
// 
// =====================================================================================

#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    printf("sleeper: %u\n", getpid());
    sleep(30);
    printf("sleeper: %u wake up\n", getpid());
    return 0;
}

