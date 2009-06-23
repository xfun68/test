// =====================================================================================/*{{{*/
// 
//       Filename:  shmq.h
// 
//    Description:  基于共享内存的循环队列
// 
//        Version:  1.0
//        Created:  06/08/2009 11:21:44 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Wang Xiaofeng (xfun68), xfun68@gmail.com
//        Company:  Linekong
// 
// =====================================================================================/*}}}*/

#ifndef _SHM_Q
#define _SHM_Q

#include "shm.h"
#include "loop_queue.h"
#include <stdlib.h>
#include <unistd.h>

const int32_t MAX_DATA_LEN = 1024;
struct DataUnit {
    int32_t len;
    int32_t fd;
    int8_t data[MAX_DATA_LEN];
};

struct SHMState
{
    int32_t be_dest_;
    uint32_t size_;
    uint32_t elem_len_;
};

template <typename T>
class SHMQ
{
public:
    SHMQ(void) :
        from_app_id_(0),
        to_app_id_(0),
        size_(0),
        be_output_queue_(true),
        state_(NULL),
        shmid_(-1),
        key_(-1)
    {
    }

    ~SHMQ(void)
    {
        release();
    }

    int32_t initialize(uint32_t from_app_id,
        uint32_t to_app_id,
        uint32_t size,
        bool be_output_queue)
    {
        int32_t result = -1;
        int32_t shmid = -1;
        void* addr = NULL;

        from_app_id_ = from_app_id;
        to_app_id_ = to_app_id;
        size_ = size;
        be_output_queue_ = be_output_queue;

        if (0 == size_) {
            goto ExitError;
        }

        if (state_ != NULL) {
            goto ExitOK;
        }

        key_ = from_app_id | (to_app_id >> 4);
        if (be_output_queue_) {
            shmid = shmCreate(key_, sizeof(SHMState) + sizeof(T) * size);
        } else {
            shmid = shmGet(key_);
        }
        if (shmid < 0) {
#ifdef DEBUG_TRACE
            PUT_ERR("#[SHMQ::initialize::shmCreate/shmGet]");
#endif //  DEBUG_TRACE
            goto ExitError;
        }

        if ((addr = shmAt(shmid)) < 0) {
#ifdef DEBUG_TRACE
            PUT_ERR("#[SHMQ::initialize::shmAt]");
#endif //  DEBUG_TRACE
            goto ExitError;
        }

        shmid_ = shmid;
        state_ = (SHMState*)addr;
        if (be_output_queue_) {
            state_->be_dest_ = false;
            state_->size_ = size_;
            state_->elem_len_ = sizeof(T);
        }

        if (queue_.setQueueAddr((int8_t*)addr+sizeof(SHMState),
                state_->elem_len_,
                state_->size_) < 0) {
#ifdef DEBUG_TRACE
            PUT_ERR("#[SHMQ::initialize::setQueueAddr]");
#endif //  DEBUG_TRACE
            goto ExitError;
        }
ExitOK:
        result = 0;
ExitError:
        return result;
    }

    int32_t initialize(void)
    {
        return initialize(from_app_id_,
            to_app_id_,
            size_,
            be_output_queue_);
    }

    int32_t release(void)
    {
        if (NULL == state_) {
            goto ExitOK;
        }

#ifdef DEBUG_TRACE
        printf("#[release key=0x%08x shmid=%d]\n",
            ftok(getenv("PWD"), from_app_id_ | (to_app_id_ >> 4)),
            shmid_);
#endif //  DEBUG_TRACE

        if (be_output_queue_) {
            state_->be_dest_ = 1;
            shmDestroy(shmid_);
        }
        shmDt(state_);
        queue_.setQueueAddr(NULL);

        /*{{{
        from_app_id_ = 0;
        to_app_id_ = 0;
        size_ = 0;
        be_output_queue_ = true;
        }}}*/
        state_ = NULL;
        shmid_ = -1;
ExitOK:
        return 0;
    }

    LoopQueue<T>& queue(void)
    {
        if (!isValid()) {
            release();
            initialize();
        }
        return queue_;
    }

    const LoopQueue<T>& queue(void) const
    {
        return queue();
    }

    bool isValid(void)
    {
        if (NULL != state_) {
            if ((queue_.size() > 0) || (0 == state_->be_dest_)) {
                return 1;
            }
        }
        return 0;
    }

private:
    uint32_t from_app_id_;
    uint32_t to_app_id_;
    uint32_t size_;
    bool be_output_queue_;
    SHMState* state_;
    int32_t shmid_;
    key_t key_;
    LoopQueue<T> queue_;
};

#endif // _SHM_Q

