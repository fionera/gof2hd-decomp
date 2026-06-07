#include "class.h"

// FileInterfaceAndroid::Read(unsigned int, void*) — reads from the zip handle, the stdio handle,
// or the JNI input stream (in which case the bytes are copied out of a Java byte[] region).
//
// NOTE: near-match only. The zip/stdio fast paths are straightforward, but the JNI path builds an
// AbyssEngine::String per byte under a stack-protector canary; that loop, the String ABI, and the
// -Oz frame layout do not reproduce byte-exact. Faithful semantic model.

extern "C" unsigned int zip_fread(void *zf, void *buf, unsigned int n);
extern "C" unsigned int fread(void *buf, unsigned int sz, unsigned int n);
extern void **gEnvR __attribute__((visibility("hidden")));
extern void *gReadMidArg __attribute__((visibility("hidden")));
extern "C" unsigned int JNI_CallIntMethod(void *env, void *m, void *arg0, void *arg1);

typedef void *(*fn_i)(void *env, unsigned int n);
typedef void (*fn_getregion)(void *env, void *arr, int start, unsigned int len, void *buf);
typedef int (*fn_check)(void *env);
typedef void (*fn_void)(void *env);
typedef void (*fn_del)(void *env, void *arr);

extern "C" bool FileInterfaceAndroid_Read(FileInterfaceAndroid *self, unsigned int n, void *buf)
{
    if (F<void *>(self, 0x0c) != 0)
        return zip_fread(F<void *>(self, 0x0c), buf, n) == n;
    if (F<void *>(self, 0x08) != 0)
        return fread(buf, 1, n) == n;
    if (F<void *>(self, 0x10) == 0)
        return false;

    void *r9 = *gEnvR;
    void *env = *(void **)r9;
    void *table = *(void **)env;
    void *arr = (*(fn_i *)((char *)table + 0x2c0))(env, n);
    unsigned int got = JNI_CallIntMethod(*(void **)r9, F<void *>(self, 0x10), *(void **)gReadMidArg, arr);

    bool ok;
    table = *(void **)(*(void **)r9);
    if ((*(fn_check *)((char *)table + 0x3c))(*(void **)r9) == 0 && got == n) {
        env = *(void **)r9;
        table = *(void **)env;
        (*(fn_getregion *)((char *)table + 0x320))(env, arr, 0, n, buf);
        ok = true;
    } else {
        table = *(void **)(*(void **)r9);
        (*(fn_void *)((char *)table + 0x40))(*(void **)r9);
        (*(fn_void *)((char *)(*(void **)(*(void **)r9)) + 0x44))(*(void **)r9);
        ok = false;
    }
    table = *(void **)(*(void **)r9);
    (*(fn_del *)((char *)table + 0x5c))(*(void **)r9, arr);
    return ok;
}
