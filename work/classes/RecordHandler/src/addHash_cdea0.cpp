#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" int RecordHandler_readRecordAsByteArray(RecordHandler *self, signed char **out, int slot, bool fromBackup);
extern "C" void RecordHandler_writeByteArrayAsRecord(RecordHandler *self, signed char *buf, unsigned n, int slot, bool toBackup);
extern "C" unsigned char *RH_op_new_arr(unsigned int n);
extern "C" void RH_op_delete_arr(void *p);
extern "C" void *RH_op_new(unsigned int sz);
extern "C" void RH_op_delete(void *p);
extern "C" void SHA256_Init(void *c);
extern "C" void SHA256_Update(void *c, const void *data, int n);
extern "C" void SHA256_Final(unsigned char *md, void *c);
extern "C" void *RH_memcpy(void *dst, const void *src, unsigned int n);

__attribute__((visibility("hidden"))) extern int *g_AH_guard;                  // DAT_000ddfa0 -> guard holder
__attribute__((visibility("hidden"))) extern const unsigned char RH_ah_salt[]; // DAT_000ddfa8 (25 bytes)
__attribute__((visibility("hidden"))) extern unsigned char **RH_ah_key;        // DAT_000ddfac (16-byte key)

// RecordHandler::addHash(int slot)
extern "C" void RecordHandler_addHash(RecordHandler *self, int slot)
{
    int *guardP = g_AH_guard;
    volatile int saved = *guardP;

    signed char *data = 0;
    int len = RecordHandler_readRecordAsByteArray(self, &data, slot, false);
    if (-1 < len) {
        unsigned char *md = RH_op_new_arr(0x20);
        void *c = RH_op_new(0x70);
        SHA256_Init(c);
        void (*update)(void *, const void *, int) = SHA256_Update;
        update(c, data, len);
        update(c, RH_ah_salt, 0x19);
        update(c, *RH_ah_key, 0x10);
        SHA256_Final(md, c);

        signed char *out = (signed char *)RH_op_new_arr(len + 0x20);
        RH_memcpy(out, data, len);
        unsigned long long *dst = (unsigned long long *)(out + len);
        unsigned long long *src = (unsigned long long *)md;
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        RecordHandler_writeByteArrayAsRecord(self, out, len + 0x20, slot, false);

        void (*del)(void *) = (void (*)(void *))RH_op_delete_arr;
        del(data);
        data = 0;
        del(out);
        del(md);
        RH_op_delete(c);
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}
