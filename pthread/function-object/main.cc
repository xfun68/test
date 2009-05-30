// =====================================================================================
// 
//       Filename:  main.cc
// 
//    Description:  测试函数对象可否作为线程开始函数
// 
//        Version:  1.0
//        Created:  05/29/2009 11:26:03 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Will King (xfun), xfun68@foxmail.com
//        Company:  
// 
// =====================================================================================

#include <time.h>
#include <pthread.h>
#include <inttypes.h>
#include <iostream>

using namespace std;

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
        perror("runInThread");
        result = retcode;
        goto ExitError;
    }

    result = 0;
ExitError:
    return result;
}

int32_t runInThread(void* (*threadFunc)(void*), void* args)
{
    int32_t result = -1;
    int32_t retcode = -1;
    pthread_t tid = 0;

    if (0 != (retcode = pthread_create(&tid, NULL,
                threadFunc, args))) {
        perror("runInThread");
        result = retcode;
        goto ExitError;
    }

    result = 0;
ExitError:
    return result;
}

class ThreadClass {
public:
    ThreadClass(void) : 
        be_release_(0) {
    }

    void run(void) {
        int32_t count = 20;
        while (count--) {
            cout << "ThreadClass count      "
                << count
                << endl;

            if (be_release_) {
                pthread_exit(NULL);
            }
            sleep(1);
        }
        return;
    }

    int32_t initialize(void) {
        int32_t result = -1;

        be_release_ = false;
        result = 0;
        return result;
    }

    int32_t release(void) {
        be_release_ = true;
        return 0;
    }

private:
    bool be_release_;
};

void* threadRun(void* args) {
    int32_t count = 20;
    while (count--) {
        cout << "threadRun count            "
            << count
            << endl;

        sleep(1);
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    int32_t result = -1;
    int32_t retcode = -1;
    ThreadClass ta;
    int32_t count = 10;

    if (0 != (retcode = ta.initialize())) {
        cout << "ThreadClass initialize with error[" << result << "]" << endl;
        result = retcode;
        goto ExitError;
    } else {
        cout << "ThreadClass initialize OK" << endl;
    }

    if (0 != (retcode = runInThread<ThreadClass>(&ta))) {
        cout << "runInThread(class) with error[" << result << "]" << endl;
        result = retcode;
        goto ExitError;
    } else {
        cout << "runInThread(class) OK" << endl;
    }

    if (0 != (retcode = runInThread(threadRun, NULL))) {
        cout << "runInThread(function) with error[" << result << "]" << endl;
        result = retcode;
        goto ExitError;
    } else {
        cout << "runInThread(function) OK" << endl;
    }

    while (count--) {
        cout << "main thread count " << count << endl;
        if (5 == count) {
            ta.release();
        }
        sleep(1); 
    }

    result = 0;
ExitError:
    if (0 == result) {
        cout << "program exit OK" << endl;
    } else {
        cout << "program exit with error[" << result << "]" << endl;
    }

    return result;
}

