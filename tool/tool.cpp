// =====================================================================================
// 
//       Filename:  tool.cpp
// 
//    Description:  һЩ���õĹ����ຯ��
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
#include <sys/resource.h>

/*
 * ���ܣ�   ��ȡ��ǰ�����ܴ򿪵�����ļ�������������
 * ����ֵ��
 *  # 0     �ɹ�
 *  # !=0   ��ǰ���õ��������
 */
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

/*
 * ���ܣ�   ���õ�ǰ�����ܴ򿪵�����ļ�������������
 * ����ֵ��
 *  # 0     �ɹ�
 *  # -1    ʧ��
 */
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

