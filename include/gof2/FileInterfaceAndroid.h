#ifndef GOF2_FILEINTERFACEANDROID_H
#define GOF2_FILEINTERFACEANDROID_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — FileInterfaceAndroid class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: on the class), with file I/O over stdio FILE*,
// libzip zip_file*, and JNI streams. Field offsets recovered per-method from the
// target disassembly; methods access fields via byte-offset casts from `this`.
//
// Layout (from the ctors / Close / Read / Write):
//   0x00 vtable
//   0x04 alive/refcount byte (set 1 in default ctor)
//   0x08 FILE*           (stdio handle)
//   0x0c zip_file*       (libzip handle)
//   0x10 jmethodID* / jobject stream handle
//   0x14 byte            (append/write-mode flag)
//   0x1c int
//   0x24 byte
//   0x28 int
//   0x30 void* AppRootDir
//   0x34 void* ZipDirectory




// AbyssEngine::String passed/returned by value: 12-byte trivially-copied aggregate.


// Field accessors via byte offset from `this`.

static inline int32_t &i32(void *self, unsigned off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, unsigned off) { return *(uint32_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, unsigned off) { return *(uint8_t *)((char *)self + off); }
static inline void *&pp(void *self, unsigned off) { return *(void **)((char *)self + off); }

struct FileInterfaceAndroid {
    char* field_0x0;                    // +0x0
    void* field_0x8;                    // +0x8
    const char* field_0x30;             // +0x30
    void* field_0x34;                   // +0x34
};
#endif
