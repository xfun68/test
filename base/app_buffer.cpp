#include "app_buffer.h"

#include <string.h>
#include <assert.h>

#include <arpa/inet.h>

// constructors and destructors
AppBuffer::AppBuffer(int32_t buf_size) :
    head_(0),
    tail_(0)
{
    if ((buf_size < MIN_BUF_SIZE) || (buf_size > MAX_BUF_SIZE)) {
        buf_size_ = DEF_BUF_SIZE;
    } else {
        buf_size_ = buf_size;
    }

    app_buf_ = new int8_t[buf_size_];
    assert(app_buf_);
    memset(app_buf_, 0, buf_size_);
}

AppBuffer::AppBuffer(const AppBuffer& rhs)
{
    buf_size_ = rhs.buf_size();
    app_buf_ = new int8_t[buf_size_];
    assert(app_buf_);
    memcpy(app_buf_, rhs.app_buf(), buf_size_);
    head_ = rhs.head();
    tail_ = rhs.tail();
}

const AppBuffer& AppBuffer::operator = (const AppBuffer& rhs)
{
    if (this == &rhs) {
        goto ExitOK;
    }

    if ((rhs.buf_size() != buf_size_) || (NULL == app_buf_)) {
        delete [] app_buf_;
        buf_size_ = rhs.buf_size();
        app_buf_ = new int8_t[buf_size_];
        assert(app_buf_);
    }
    memcpy(app_buf_, rhs.app_buf(), buf_size_);
    head_ = rhs.head();
    tail_ = rhs.tail();
    
ExitOK:
    return *this;
}

AppBuffer::~AppBuffer(void)
{
    if (NULL != app_buf_) {
        delete [] app_buf_;
        app_buf_ = NULL;
    }

    buf_size_ = 0;
    head_ = 0;
    tail_ = 0;
}

// options
int32_t AppBuffer::peek(void* buf, int32_t size)
{
    int32_t result = -1;
    int32_t min_size = 0;
    int32_t head = 0;
    int32_t tail = 0;
    int8_t* dst_buf = NULL;

    if (size <= 0) {
        result = -1;
        goto ExitError;
    }

    if ((tail_ - head_) <= 0) {
        result = 0;
        goto ExitError;
    }

    dst_buf = (int8_t*)buf;
    min_size = (size <= (tail_ - head_) ? size : (tail_ - head_));
    head = head_ % buf_size_;
    tail = tail_ % buf_size_;

    if (head <= tail) {
        memcpy(dst_buf, app_buf_ + head, min_size);
    } else {
        if (min_size <= (buf_size_ - head)) {
            memcpy(dst_buf, app_buf_ + head, min_size);
        } else {
            memcpy(dst_buf, app_buf_ + head, buf_size_ - head);
            memcpy(dst_buf + (buf_size_ - head), app_buf_, min_size - (buf_size_ - head));
        }
    }

    result = min_size;
ExitError:
    return result;
}

int32_t AppBuffer::seek(int32_t size)
{
    if (size >= 0) {
        return head_ += size;
    } else {
        return head_;
    }
}

int32_t AppBuffer::read(void* buf, int32_t size)
{
    int32_t result = -1;
    if ((result = peek(buf, size)) > 0) {
        head_ += result;
        if (head_ == tail_) {
            clear();
        }
    }
    return result;
}

int32_t AppBuffer::peekUInt16(uint16_t& value)
{
    int32_t result = -1;
    if ((result = peek(&value, sizeof(value))) > 0) {
        value = ntohs(value);
    } else {
        value = 0;
    }
    return result;
}

int32_t AppBuffer::write(const void* buf, int32_t size)
{
    int32_t result = -1;
    int32_t head = 0;
    int32_t tail = 0;
    int8_t* src_buf = NULL;

    if (size <= 0) {
        result = -1;
        goto ExitError;
    }
    if (freeSize() < size) {
        result = 0;
        goto ExitError;
    }

    src_buf = (int8_t*)buf;
    head = head_ % buf_size_;
    tail = tail_ % buf_size_;
    if (head <= tail) {
        if (size <= (buf_size_ - tail)) {
            memcpy(app_buf_ + tail, src_buf, size);
        } else {
            memcpy(app_buf_ + tail, src_buf, buf_size_ - tail);
            memcpy(app_buf_, src_buf + (buf_size_ - tail), size - (buf_size_ - tail));
        }
    } else {
        memcpy(app_buf_ + tail, src_buf, size);
    }

    tail_ += size;
    result = size;
ExitError:
    return result;
}

const int8_t* AppBuffer::app_buf(void) const
{
    return app_buf_;
}

int32_t AppBuffer::buf_size(void) const
{
    return buf_size_;
}

uint32_t AppBuffer::head(void) const
{
    return head_ % buf_size_;
}

uint32_t AppBuffer::tail(void) const
{
    return tail_ % buf_size_;
}

int32_t AppBuffer::dataSize(void) const
{
    return (tail_ - head_);
}

int32_t AppBuffer::freeSize(void) const
{
    return (buf_size_ - (tail_ - head_));
}

void AppBuffer::clear(void)
{
    head_ = tail_ = 0;
}
 
