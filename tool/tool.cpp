// =====================================================================================
// 
//       Filename:  tool.cpp
// 
//    Description:  一些常用的工具类函数
// 
//        Version:  1.0
//        Created:  05/28/09 16:40:20
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Will King (xfun), xfun68@foxmail.com
//        Company:  
// 
// =====================================================================================

#include "tool.h"
#include <unistd.h>
#include <sys/resource.h>

int32_t getMaxFDSize(void)
{
    struct rlimit rt;

    rt.rlim_max = rt.rlim_cur = 0;
    if (getrlimit(RLIMIT_NOFILE, &rt) == -1) {
        return -1;
    } else {
        return (int32_t)rt.rlim_cur;
    }
}

int32_t setMaxFDSize(int32_t size)
{
    struct rlimit rt;
    rt.rlim_max = rt.rlim_cur = size;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1) {
        return -1;
    } else {
        return 0;
    }
}

