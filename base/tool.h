// =====================================================================================
//
//       Filename:  tool.h
//
//    Description:  Ò»Ğ©³£ÓÃµÄ¹¤¾ßÀàº¯Êı¼¯ºÏ
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

#include <string.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <inttypes.h>

#define ASSURE_SUCCESS(RESULT, RETCODE, STATEMENT); \
    if (S_SUCCESS != (RETCODE = (STATEMENT))) {\
        RESULT = RETCODE;\
        goto ExitError;\
    }

#define PUT_ERR(TAG); \
    printf("%s: [%d] %s\n", TAG, errno, strerror(errno));

/*
 * ¹¦ÄÜ£º   »ñÈ¡µ±Ç°½ø³ÌÄÜ´ò¿ªµÄ×î´óÎÄ¼şÃèÊö·ûµÄÊıÁ¿
 * ·µ»Ø£º
 *  # 0     ³É¹¦
 *  # !=0   µ±Ç°ÉèÖÃµÄ×î´óÊıÁ¿
 */
int32_t getMaxFDSize(void);
/*
 * ¹¦ÄÜ£º   ÉèÖÃµ±Ç°½ø³ÌÄÜ´ò¿ªµÄ×î´óÎÄ¼şÃèÊö·ûµÄÊıÁ¿
 * ·µ»Ø£º
 *  # 0     ³É¹¦
 *  # -1    Ê§°Ü
 */
int32_t setMaxFDSize(int32_t size);

/*
 * ¹¦ÄÜ£º   ±íÊ¾µ±Ç°Ê±¼äµÄ×Ö·û´®££¨º¬Î¢Ãë£©
 */
const char* timeString(void);

/*
 * ¹¦ÄÜ£º   Ïß³Ìº¯ÊıÄ£°å
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
 * ¹¦ÄÜ£º   ×ª»»ÕûĞÍÊ±¼äÎª¿É¶Á×Ö·û´®Ê±¼ä
 */
const char* time2Str(time_t tm = time(NULL));

/*
 * ¹¦ÄÜ£º   ½ø³ÌÊµÀıÎ¨Ò»Ô¼Êø
 */
void instanceRestrict(const char* arg0);

/*
 * ÈÕÖ¾Êä³öº¯Êı
 */
void logger(const uint32_t zone,
    const uint32_t gateway,
    const char*    type,
    const int      ret,
    const uint32_t user_id,
    const uint32_t role_id,
    const char*    format,
    ...);
void logger(const char* format, ...);

#endif //  _TOOL_H_

