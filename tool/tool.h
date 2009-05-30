// =====================================================================================
// 
//       Filename:  tool.h
// 
//    Description:  一些常用的工具类函数集合
// 
//        Version:  1.0
//        Created:  05/28/09 16:41:02
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Will King (xfun), xfun68@foxmail.com
//        Company:  
// 
// =====================================================================================

#ifndef _TOOL_H_

#include <time.h>
#include <pthread.h>
#include <inttypes.h>

/*
 * 功能：   获取当前进程能打开的最大文件描述符的数量
 * 返回：
 *  # 0     成功
 *  # !=0   当前设置的最大数量
 */
int32_t getMaxFDSize(void);
/*
 * 功能：   设置当前进程能打开的最大文件描述符的数量
 * 返回：
 *  # 0     成功
 *  # -1    失败
 */
int32_t setMaxFDSize(int32_t size);

/*
 * 功能：   线程函数模板
 */
template <class T >
void* threadFunc(void* args)
{
    T* thread_class = (T*)args;
    thread_class->runInThread();
    return NULL;
}

#endif //  _TOOL_H_

