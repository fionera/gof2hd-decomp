#include "class.h"

extern "C" int fclose(void *f);
extern "C" int zip_fclose(void *zf);
// _JNIEnv::CallVoidMethod(jobject env, jmethodID m, jobject arg) — variadic JNI stub.
extern "C" void JNI_CallVoidMethod(void *env, void *m, void *arg);

// JNI env slot + the two cached jstring constants (append vs write mode), all PC-relative.
extern void *gJniEnv __attribute__((visibility("hidden")));
extern void *gModeWrite __attribute__((visibility("hidden")));
extern void *gModeAppend __attribute__((visibility("hidden")));

// FileInterfaceAndroid::Close() — close whichever handle is open; for a JNI stream, notify Java.
// NOTE: near-match. The fclose/zip_fclose/handle-clear logic is byte-identical; the only
// divergence is the mode-string select: the target branches to load one of two PC-relative
// jstring globals, while clang -Oz pre-loads both and uses a conditional move (it ne; movne),
// which also shifts a callee-saved register (r5 vs r6). A -Oz branch-vs-cmov choice.
extern "C" void FileInterfaceAndroid_Close(FileInterfaceAndroid *self)
{
    if (F<void *>(self, 0x08) != 0) {
        fclose(F<void *>(self, 0x08));
        F<void *>(self, 0x08) = 0;
    }
    if (F<void *>(self, 0x0c) != 0) {
        zip_fclose(F<void *>(self, 0x0c));
        F<void *>(self, 0x0c) = 0;
    }
    void *m = F<void *>(self, 0x10);
    if (m != 0) {
        void *env = *(void **)gJniEnv;
        void *modePtr;
        if (u8(self, 0x14) == 0)
            modePtr = gModeWrite;
        else
            modePtr = gModeAppend;
        JNI_CallVoidMethod(env, m, *(void **)modePtr);
        F<void *>(self, 0x10) = 0;
    }
}
