// =====================================================================================
// 
//       Filename:  main.c
// 
//    Description:  ²âÊÔusleepºÄÊ±Çé¿ö
// 
//        Version:  1.0
//        Created:  06/26/2009 03:44:29 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Wang Xiaofeng (xfun68), xfun68@gmail.com
//        Company:  Linekong
// 
// =====================================================================================

#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[])
{
    int i = 0;
    const int MAX_TIMES = 10000;

    puts("---------------------------- start ----------------------------");

    while (i++ < MAX_TIMES) {
        usleep(1);
    }

    puts("---------------------------- stop ----------------------------");

    return 0;
}

