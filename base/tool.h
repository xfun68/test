// =====================================================================================
//
//       Filename:  tool.h
//
//    Description:  һЩ���õĹ����ຯ������
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
 * ���ܣ�   ��ȡ��ǰ�����ܴ򿪵�����ļ�������������
 * ���أ�
 *  # 0     �ɹ�
 *  # !=0   ��ǰ���õ��������
 */
int32_t getMaxFDSize(void);
/*
 * ���ܣ�   ���õ�ǰ�����ܴ򿪵�����ļ�������������
 * ���أ�
 *  # 0     �ɹ�
 *  # -1    ʧ��
 */
int32_t setMaxFDSize(int32_t size);

/*
 * ���ܣ�   ��ʾ��ǰʱ����ַ��������΢�룩
 */
const char* timeString(void);

/*
 * ���ܣ�   �̺߳���ģ��
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
 * ���ܣ�   ת������ʱ��Ϊ�ɶ��ַ���ʱ��
 */
const char* time2Str(time_t tm = time(NULL));

/*
 * ���ܣ�   ����ʵ��ΨһԼ��
 */
void instanceRestrict(const char* arg0);

/*
 * ��־�������
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

