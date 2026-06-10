#ifndef GOF2_NFC_H
#define GOF2_NFC_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
typedef void *(*NFC_FindClassFn)(void *, void *);
typedef void *(*NFC_GetStaticMethodFn)(void *, void *, const char *, const char *);

static inline void *nfc_jni_slot(void *env, unsigned offset)
{
    return *(void **)((char *)*(void **)env + offset);
}

static inline void *nfc_find_class(void *env, void *name)
{
    return ((NFC_FindClassFn)nfc_jni_slot(env, 0x18))(env, name);
}

static inline void *nfc_get_static_method(void *env, void *cls, const char *name, const char *sig)
{
    return ((NFC_GetStaticMethodFn)nfc_jni_slot(env, 0x1c4))(env, cls, name, sig);
}

extern "C" void NFC_CallStaticVoidMethod(void *env, void *cls, void *method);
extern "C" int NFC_CallStaticBooleanMethod(void *env, void *cls, void *method);
extern "C" int NFC_CallStaticIntMethod(void *env, void *cls, void *method);
extern "C" void NFC_DeleteLocalRef(void *env);

struct NFC { void* _opaque; };  // no offset accesses observed
#endif
