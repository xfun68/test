// =====================================================================================
// 
//       Filename:  main.cc
// 
//    Description:  测试字符串转整数
// 
//        Version:  1.0
//        Created:  07/14/2009 09:07:13 PM
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

void reset(char* str, int32_t size)
{
    memset(str, ' ', size);
    str[size-1] = '\0';
}

int main(int argc, char* argv[])
{
    const int32_t MAX_VALUE_LEN = 11;
    char string[MAX_VALUE_LEN] = {' '};

    for (uint32_t i = 1; i < sizeof(string); ++i) {
        putchar('|');
    }
    putchar('\n');

    int32_t ivalue = 0;

    // 没有数字的
    reset(string, sizeof(string));
    ivalue = strtol(string, NULL, 0);
    printf("%s -> %d\n", string, ivalue);

    // 前面是空格的
    reset(string, sizeof(string));
    string[9] = '9';
    ivalue = strtol(string, NULL, 0);
    printf("%s -> %d\n", string, ivalue);

    reset(string, sizeof(string));
    string[8] = '-';
    string[9] = '9';
    ivalue = strtol(string, NULL, 0);
    printf("%s -> %d\n", string, ivalue);

    // 后面是空格的
    reset(string, sizeof(string));
    string[0] = '9';
    ivalue = strtol(string, NULL, 0);
    printf("%s -> %d\n", string, ivalue);

    reset(string, sizeof(string));
    string[0] = '-';
    string[1] = '9';
    ivalue = strtol(string, NULL, 0);
    printf("%s -> %d\n", string, ivalue);

    // 两边是空格的
    reset(string, sizeof(string));
    string[5] = '9';
    ivalue = strtol(string, NULL, 0);
    printf("%s -> %d\n", string, ivalue);

    reset(string, sizeof(string));
    string[4] = '-';
    string[5] = '9';
    ivalue = strtol(string, NULL, 0);
    printf("%s -> %d\n", string, ivalue);

    // 混乱的字符串
    reset(string, sizeof(string));
    string[0] = '-';
    string[3] = '3';
    string[5] = '+';
    ivalue = strtol(string, NULL, 0);
    printf("%s -> %d\n", string, ivalue);

    reset(string, sizeof(string));
    string[9] = '+';
    ivalue = strtol(string, NULL, 0);
    printf("%s -> %d\n", string, ivalue);

    reset(string, sizeof(string));
    string[4] = '-';
    string[5] = '+';
    ivalue = strtol(string, NULL, 0);
    printf("%s -> %d\n", string, ivalue);

    // 更~~~混乱的字符串
    reset(string, sizeof(string));
    string[0] = '-';
    string[3] = '3';
    string[5] = '+';
    string[9] = '@';
    string[10] = '@';
    ivalue = strtol(string, NULL, 0);
    printf("%s -> %d\n", string, ivalue);

}
