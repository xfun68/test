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
#define _TOOL_H_

#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <inttypes.h>

#define ASSURE_SUCCESS(RESULT, RETCODE, STATEMENT) \
    if (S_SUCCESS != (RETCODE = (STATEMENT))) {\
        RESULT = RETCODE;\
        goto ExitError;\
    }

#define PUT_ERR(TAG) \
    printf("%s: [%d] %s\n", TAG, errno, strerror(errno));

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
    thread_class->run();
    return NULL;
}

template <class T >
int32_t runInThread(void* args)
{
    int32_t result = -1;
    int32_t retcode = -1;
    pthread_t tid = 0;

    if (0 != (retcode = pthread_create(&tid, NULL,
                threadFunc<T>, args))) {
        result = retcode;
        goto ExitError;
    }

    result = 0;
ExitError:
    return result;
}

int32_t runInThread(void* (*threadFunc)(void*), void* args);

/*
 * 功能：   转换整型时间为可读字符串时间
 */
const char* time2Str(time_t tm = time(NULL));

/*
 * 功能：   进程实例唯一约束
 */
void instanceRestrict(const char* arg0);

#endif //  _TOOL_H_

