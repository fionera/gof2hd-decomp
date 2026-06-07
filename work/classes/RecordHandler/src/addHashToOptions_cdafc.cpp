#include "class.h"

// SHA salt blob (25 bytes) and the second 16-byte key, both via hidden pc-rel relocs.
__attribute__((visibility("hidden"))) extern const unsigned char RH_aho_salt[];
__attribute__((visibility("hidden"))) extern unsigned char **RH_aho_key;

extern "C" int RecordHandler_readOptionsFileAsByteArray(RecordHandler *self, signed char **out);
extern "C" unsigned char *RH_op_new_arr(unsigned int n);
extern "C" void *RH_op_new(unsigned int sz);
extern "C" void SHA256_Init(void *c);
extern "C" void SHA256_Update(void *c, const void *data, int n);
extern "C" void SHA256_Final(unsigned char *md, void *c);
extern "C" void *RH_memcpy(void *dst, const void *src, unsigned int n);
extern "C" void RecordHandler_writeByteArrayAsOptionsFile(RecordHandler *self, signed char *buf, int n);
extern "C" void RH_op_delete_arr(void *p);
extern "C" void RH_op_delete(void *p);

// RecordHandler::addHashToOptions()
extern "C" void RecordHandler_addHashToOptions(RecordHandler *self)
{
    signed char *data = 0;
    int len = RecordHandler_readOptionsFileAsByteArray(self, &data);
    if (len < 0)
        return;

    unsigned char *md = RH_op_new_arr(0x20);
    void *c = RH_op_new(0x70);
    SHA256_Init(c);
    void (*update)(void *, const void *, int) = SHA256_Update;
    update(c, data, len);
    update(c, RH_aho_salt, 0x19);
    update(c, *RH_aho_key, 0x10);
    SHA256_Final(md, c);

    signed char *out = (signed char *)RH_op_new_arr(len + 0x20);
    RH_memcpy(out, data, len);
    unsigned long long *dst = (unsigned long long *)(out + len);
    unsigned long long *src = (unsigned long long *)md;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    RecordHandler_writeByteArrayAsOptionsFile(self, out, len + 0x20);

    void (*del)(void *) = (void (*)(void *))RH_op_delete_arr;
    del(data);
    data = 0;
    del(out);
    del(md);
    RH_op_delete(c);
}
