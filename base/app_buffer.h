#ifndef _APP_BUFFER_H_
#define _APP_BUFFER_H_

#include "OperationCode.h"
#include "inttypes.h"

class AppBuffer
{
public:
    enum BufSize {
        MIN_BUF_SIZE = 16 * 1024,    // 1K
        DEF_BUF_SIZE = 32 * 1024,    // 32K
        MAX_BUF_SIZE = 64 * 1024     // 64K
    };

public:
    // constructors and destructors
    AppBuffer(int32_t buf_size = DEF_BUF_SIZE);
    AppBuffer(const AppBuffer& rhs);
    const AppBuffer& operator = (const AppBuffer& rhs);
    ~AppBuffer(void);

    // options
    int32_t read(void* buf, int32_t size);
    int32_t peek(void* buf, int32_t size);
    int32_t seek(int32_t size);
    int32_t peekUInt16(uint16_t& value);
    int32_t write(const void* buf, int32_t size);
    int32_t dataSize(void) const;
    int32_t freeSize(void) const;
    void clear(void);

    const int8_t* app_buf(void) const; 
    int32_t buf_size(void) const;
    uint32_t head(void) const;
    uint32_t tail(void) const;

private:
    // attributes
    int8_t* app_buf_;
    int32_t buf_size_;
    int32_t head_;
    int32_t tail_;
};

#endif // _APP_BUFFER_H_

