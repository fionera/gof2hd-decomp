#ifndef WORK_CLASSES_GAMERECORD_SRC_CLASS_H
#define WORK_CLASSES_GAMERECORD_SRC_CLASS_H

// Galaxy on Fire 2 - GameRecord (top-level class, no AbyssEngine:: namespace).
// Embeds two AbyssEngine::String members at +0x188 and +0x194, plus a heap buffer
// at +0x00 and many scalar fields. Fields reached via byte-offset casts from `this`.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;

struct GameRecord {
    GameRecord();
    ~GameRecord();
    void load();
};

// Field accessor via byte offset.
template <class T>
static inline T &F(void *self, unsigned off) { return *(T *)((char *)self + off); }

extern "C" {
void *operator_new__(uint32_t size);
void operator_delete__(void *ptr);
void __aeabi_memcpy(void *dst, const void *src, uint32_t n);
void __aeabi_memclr8(void *dst, uint32_t n);
void __aeabi_memclr4(void *dst, uint32_t n);
void __aeabi_memclr(void *dst, uint32_t n);

// AbyssEngine::String methods operating on a raw String slot (12-byte aggregate).
void *AEString_ctor(void *self);
void *AEString_ctor_cstr(void *self, const char *text, bool copy);
void *AEString_assign(void *self, const void *other);
void AEString_dtor(void *self);
uint32_t AEString_GetStringLength(const char *s);
}

#endif
