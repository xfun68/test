#ifndef __AES_H__
#define __AES_H__
#define AES_MIN_KEY_SIZE    16
#define AES_MAX_KEY_SIZE    32

#define AES_BLOCK_SIZE        16

#include <inttypes.h>

typedef unsigned char       u8;
typedef signed char         s8;
typedef signed short        s16;
typedef unsigned short      u16;
typedef signed int          s32;
typedef unsigned int        u32;
typedef int64_t             s64;
typedef uint64_t            u64;
typedef u16                 __le16;
typedef u32                 __le32;

#define E_KEY    (&ctx->buf[0])
#define D_KEY    (&ctx->buf[60])
#define le32_to_cpu
#define cpu_to_le32

struct aes_ctx
{
    int key_length;
    u32 buf[120];
};

extern struct aes_ctx g_aes;

int aes_init(void);
void gen_tabs(void);
int aes_set_key(struct aes_ctx * ctx, const u8 *in_key, unsigned int key_len);
void aes_encrypt(struct aes_ctx * ctx, u8 *out, const u8 *in);
void aes_decrypt(struct aes_ctx * ctx, u8 *out, const u8 *in);
int AesEncrypt(unsigned char* in, unsigned char* result, int len);
int AesDecrypt(unsigned char* in,unsigned char* result);

#endif // __AES_H__
