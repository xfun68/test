// =====================================================================================
// 
//       Filename:  main.cc
// 
//    Description:  测试sscanf将连续的字符串转为整数
// 
//        Version:  1.0
//        Created:  07/15/09 12:18:57
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Wang Xiaofeng (xfun68), xfun68@gmail.com
//        Company:  Linekong
// 
// =====================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

int main(int argc, char* argv[])
{
    char string[] = "1234abcf";

    uint32_t a = 0;

    printf("a = %u\n", a);

    sscanf(string, "%2x", &a);
    printf("a = 0x%x\n", a);

    sscanf(string+2, "%2x", &a);
    printf("a = 0x%x\n", a);

    sscanf(string+4, "%2x", &a);
    printf("a = 0x%x\n", a);

    sscanf(string+6, "%2x", &a);
    printf("a = 0x%x\n", a);

    return 0;
}

