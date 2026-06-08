#include "class.h"

extern "C" unsigned int fwrite(const void *p, unsigned int sz, unsigned int n);

// JNI: env holder global. r9 = *gEnvW; the jobject is *r9; its function table is *(jobject).
// Slots used: 0x2c0 NewByteArray, 0x340 SetByteArrayRegion, 0x3c ExceptionCheck,
// 0x44 ExceptionClear, 0x5c DeleteLocalRef.
extern void **gEnvW __attribute__((visibility("hidden")));
extern void *gWriteMidArg __attribute__((visibility("hidden")));
extern "C" void JNI_CallVoidMethod(void *env, void *m, void *arg0, void *arg1);

typedef void *(*fn_i)(void *env, unsigned int n);
typedef void (*fn_setregion)(void *env, void *arr, int start, unsigned int len, const void *buf);
typedef int (*fn_check)(void *env);
typedef void (*fn_void)(void *env);
typedef void (*fn_del)(void *env, void *arr);

// FileInterfaceAndroid::Write(unsigned int, void const*) — stdio fast path, else JNI byte stream.
// NOTE: near-match. The diff harness only compares up to the first `pop {..pc}` (the stdio
// fast-path return). That fast path is correct but its callee-saved register coloring (r11 vs
// r10) and result register (r0 vs r5) are forced by the whole-function allocation that also
// serves the JNI slow path; clang -Oz colors them differently from the target. Resistant.
extern "C" bool FileInterfaceAndroid_Write(FileInterfaceAndroid *self, unsigned int n, const void *buf)
{
    if (F<void *>(self, 0x08) != 0)
        return fwrite(buf, 1, n) == n;
    if (F<void *>(self, 0x10) == 0)
        return true;

    void *r9 = *gEnvW;
    void *envObj = *(void **)r9;
    void *table = *(void **)envObj;
    void *arr = (*(fn_i *)((char *)table + 0x2c0))(envObj, n);
    envObj = *(void **)r9;
    table = *(void **)envObj;
    (*(fn_setregion *)((char *)table + 0x340))(envObj, arr, 0, n, buf);
    JNI_CallVoidMethod(envObj, F<void *>(self, 0x10), *(void **)gWriteMidArg, arr);
    table = *(void **)(*(void **)r9);
    bool ok = (*(fn_check *)((char *)table + 0x3c))(*(void **)r9) == 0;
    if (!ok)
        (*(fn_void *)((char *)(*(void **)(*(void **)r9)) + 0x44))(*(void **)r9);
    table = *(void **)(*(void **)r9);
    (*(fn_del *)((char *)table + 0x5c))(*(void **)r9, arr);
    return ok;
}
