#include "class.h"

extern "C" int AEFile_GetFileSize(unsigned int fd);
extern "C" void AEFile_Read(unsigned int n, unsigned char *buf, unsigned int fd);
extern "C" unsigned char *RH_op_new_arr(unsigned int n);
extern "C" void RH_op_delete_arr(void *p);
extern "C" void *RH_op_new(unsigned int sz);
extern "C" void RH_op_delete(void *p);
extern "C" void Array_UC_ctor(void *a);
extern "C" void ArrayAdd_UC(unsigned char *src, unsigned int n, void *a);
extern "C" void SHA256_Init(void *c);
extern "C" void SHA256_Update(void *c, const void *data, int n);
extern "C" void SHA256_Final(unsigned char *md, void *c);

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

__attribute__((visibility("hidden"))) extern int *g_CH_guard;          // DAT_000dd8b8 -> guard holder
__attribute__((visibility("hidden"))) extern const unsigned char RH_ch_salt[]; // DAT_000dd8c0 (25 bytes)
__attribute__((visibility("hidden"))) extern unsigned char **RH_ch_key;        // DAT_000dd8c4 (16-byte key)

// RecordHandler::checkHash(unsigned int fd) -> bool (1=valid, 0=invalid).
extern "C" int RecordHandler_checkHash(unsigned int fd)
{
    int *guardP = g_CH_guard;
    volatile int saved = *guardP;
    int result = 0;

    int sz = AEFile_GetFileSize(fd);
    if (-1 < sz) {
        unsigned char *buf = RH_op_new_arr((unsigned)sz);
        AEFile_Read((unsigned)sz, buf, fd);
        void *arr = RH_op_new(0xc);
        Array_UC_ctor(arr);
        ArrayAdd_UC(buf, (unsigned)sz, arr);
        RH_op_delete_arr(buf);

        unsigned len = *(unsigned *)arr;
        if (0x21 < len) {
            unsigned char *md = RH_op_new_arr(0x20);
            void *c = RH_op_new(0x70);
            SHA256_Init(c);
            unsigned char *data = *(unsigned char **)((char *)arr + 4);
            void (*update)(void *, const void *, int) = SHA256_Update;
            update(c, data, (int)len - 0x20);
            update(c, RH_ch_salt, 0x19);
            update(c, *RH_ch_key, 0x10);
            SHA256_Final(md, c);

            unsigned i = 0;
            // compare 32 computed bytes against the trailing 32 bytes of the record
            while (i < 0x20) {
                unsigned char want = data[(int)len + i - 0x20];
                if (md[i] != want) break;
                i++;
            }
            result = (i > 0x1f) ? 1 : 0;
            RH_op_delete_arr(md);
            RH_op_delete(c);
        }
    }

    if (*guardP == saved) {
        return result;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}
