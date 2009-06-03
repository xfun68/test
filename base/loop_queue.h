#ifndef _LOOP_QUEUE_H_
#define _LOOP_QUEUE_H_

#include <inttypes.h>

template <typename T>
class LoopQueue {
public:
    typedef T ElemType;
    typedef T* ElemTypePtr;
    typedef T& ElemTypeRef;
    typedef struct _QueueHdr_ {
        uint32_t capacity_;
        uint32_t elem_size_;
        uint32_t head_;
        uint32_t tail_;
    } QueueHdr, *QueueHdrPtr, &QueueHdrRef;

public:
    LoopQueue(void) : hdr_(NULL), data_(NULL) {}

    LoopQueue(void* const addr) {
        setQueueAddr(addr);
    }

    ~LoopQueue(void) {
        hdr_ = NULL;
        data_ = NULL;
    }

    ElemTypeRef front(void) {
        if (NULL == hdr_) {
            return def_msg_;
        } else {
            if (rsize() < 0) {
                return def_msg_;
            } else {
                return at(head());
            }
        }
    }

    const ElemTypeRef front(void) const {
        return front();
    }

    ElemTypeRef back(void) {
        if (NULL == hdr_) {
            return def_msg_;
        } else {
            if (rsize() <= 0) {
                return def_msg_;
            } else {
                return at(tail() - 1);
            }
        }
    }

    const ElemTypeRef back(void) const {
        return back();
    }

    ElemTypeRef end(void) {
        if (NULL == hdr_) {
            return def_msg_;
        } else {
            if ((rsize() <= 0) || (rsize() >= capacity())) {
                return front();
            } else {
                return at(hdr_->tail_);
            }
        }
    }

    const ElemTypeRef end(void) const {
        return end();
    }
    ElemTypeRef at(uint32_t pos) {
        if (NULL == hdr_) {
            return def_msg_;
        } else {
            return data_[pos % capacity()];
        }
    }

    const ElemTypeRef at(uint32_t pos) const {
        return at(pos);
    }

    int32_t pop(void) {
        if (NULL == hdr_) {
            return -1;
        } else {
            if (rsize() <= 0) {
                return -1;
            } else {
                hdr_->head_ = head()+1;
                return 0;
            }
        }
    }

    int32_t push(const ElemTypeRef val) {
        if (NULL == hdr_) {
            return -1;
        } else {
            if (&val != &(end())) {
                end() = val;
            }
            hdr_->tail_ = tail()+1;
            return 0;
        }
    }

    int32_t clear(void) {
        if (NULL == hdr_) {
            return -1;
        } else {
            return hdr_->head_ = hdr_->tail_ = 0;
        }
    }

    bool empty(void) const {
        return size() == 0;
    }

    uint32_t capacity(void) const {
        if (NULL == hdr_) {
            return 0;
        } else {
            return hdr_->capacity_;
        }
    }

    uint32_t size(void) const {
        return rsize() > capacity() ? capacity() : rsize();
    }

    uint32_t rsize(void) const {
        if (NULL == hdr_) {
            return 0;
        } else {
            return hdr_->tail_ - hdr_->head_;
        }
    }

    uint32_t head(void) const {
        if (NULL == hdr_) {
            return -1;
        } else {
            if (rsize() > capacity()) {
                return hdr_->tail_ - capacity();
            } else {
                return hdr_->head_;
            }
        }
    }

    uint32_t tail(void) const {
        if (NULL == hdr_) {
            return -1;
        } else {
            if (rsize() < 0) {
                return hdr_->head_;
            } else {
                return hdr_->tail_;
            }
        }
    }

    int32_t setQueueAddr(void* const addr) {
        if (NULL == addr) {
            hdr_ = NULL;
            data_ = NULL;
        } else {
            hdr_ = (QueueHdrPtr)addr;
            data_ = (ElemTypePtr)((int8_t*)addr + sizeof(QueueHdr));
        }
        return 0;
    }

    int32_t setQueueAddr(void* const addr, size_t elem_size, uint32_t capacity) {
        if (NULL == addr) {
            return -1;
        } else {
            setQueueAddr(addr);
            hdr_->capacity_ = capacity;
            hdr_->elem_size_ = elem_size;
            return 0;
        }
    }

private:
    LoopQueue(const LoopQueue& rhs);
    LoopQueue& operator = (const LoopQueue& rhs);

    QueueHdrPtr hdr_;
    ElemTypePtr data_;
    ElemType def_msg_;
};

#endif //  _LOOP_QUEUE_H_

