#include "class.h"

struct jobject;
// JNI env: *env is a function table; we call slot 0x7c (GetObjectClass) and 0x84 (GetMethodID).
typedef void *(*jni_getclass)(void *env);
typedef void *(*jni_getmethod)(void *env, void *cls, const char *name, const char *sig);

extern void *gFIAVtable_obj __attribute__((visibility("hidden")));
extern int *gFIAInstCount_obj __attribute__((visibility("hidden")));
extern void **gJniEnvObj __attribute__((visibility("hidden")));

// First lazily-cached method-id cell per mode (reading vs writing) + (name,sig) literals.
extern void **gMidA_read __attribute__((visibility("hidden")));
extern const char *gNmA_read __attribute__((visibility("hidden")));
extern const char *gSgA_read __attribute__((visibility("hidden")));
extern void **gMidA_write __attribute__((visibility("hidden")));
extern const char *gNmA_write __attribute__((visibility("hidden")));
extern const char *gSgA_write __attribute__((visibility("hidden")));
// Second cell, selected by mode, plus its literals.
extern void **gMidB_read __attribute__((visibility("hidden")));
extern void **gMidB_write __attribute__((visibility("hidden")));
extern const char *gNmB __attribute__((visibility("hidden")));
extern const char *gSgB __attribute__((visibility("hidden")));

// FileInterfaceAndroid::FileInterfaceAndroid(_jobject*, bool) — JNI stream; lazily resolves and
// caches the method IDs it will use. Returns `this`.
// NOTE: near-match only. The field init + instance-count bump match, but the JNI env / function-
// table indirection levels and the three lazily-cached method-id cells do not reproduce the
// target's exact global-deref chains and -Oz scheduling. Left as a faithful semantic model.
extern "C" FileInterfaceAndroid *
FileInterfaceAndroid_ctor_obj(FileInterfaceAndroid *self, jobject *stream, bool reading)
{
    void *env = *gJniEnvObj;
    int *cnt = gFIAInstCount_obj;
    self->f_8 = 0;
    self->f_c = 0;
    self->f_10 = stream;
    self->f_14 = reading;
    self->f_0 = (char *)gFIAVtable_obj + 8;
    *cnt = *cnt + 1;
    void *cls = (*(jni_getclass *)((char *)*(void **)env + 0x7c))(env);

    void **selB;
    if (reading) {
        if (*gMidA_read == 0)
            *gMidA_read = (*(jni_getmethod *)((char *)*(void **)env + 0x84))(env, cls, gNmA_read, gSgA_read);
        selB = gMidB_read;
    } else {
        if (*gMidA_write == 0)
            *gMidA_write = (*(jni_getmethod *)((char *)*(void **)env + 0x84))(env, cls, gNmA_write, gSgA_write);
        selB = gMidB_write;
    }
    if (*selB == 0)
        *selB = (*(jni_getmethod *)((char *)*(void **)env + 0x84))(env, cls, gNmB, gSgB);
    return self;
}
