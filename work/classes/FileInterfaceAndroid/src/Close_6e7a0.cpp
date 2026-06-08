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
    if (self->f_8 != 0) {
        fclose(self->f_8);
        self->f_8 = 0;
    }
    if (self->f_c != 0) {
        zip_fclose(self->f_c);
        self->f_c = 0;
    }
    void *m = self->f_10;
    if (m != 0) {
        void *env = *(void **)gJniEnv;
        void *modePtr;
        if (self->f_14 == 0)
            modePtr = gModeWrite;
        else
            modePtr = gModeAppend;
        JNI_CallVoidMethod(env, m, *(void **)modePtr);
        self->f_10 = 0;
    }
}
