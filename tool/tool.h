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

#include <time.h>
#include <pthread.h>
#include <inttypes.h>

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
 * ���ܣ�   �̺߳���ģ��
 */
template <class T >
void* threadFunc(void* args)
{
    T* thread_class = (T*)args;
    thread_class->runInThread();
    return NULL;
}

#endif //  _TOOL_H_

