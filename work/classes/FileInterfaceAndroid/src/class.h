#ifndef WORK_CLASSES_FILEINTERFACEANDROID_SRC_CLASS_H
#define WORK_CLASSES_FILEINTERFACEANDROID_SRC_CLASS_H

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

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct FileInterfaceAndroid {
    ~FileInterfaceAndroid();   // deleting dtor authored in _FileInterfaceAndroid_6dfce.cpp
};

// AbyssEngine::String passed/returned by value: 12-byte trivially-copied aggregate.
struct String12 { uint32_t a, b, c; };

// Field accessors via byte offset from `this`.
template <class T>
static inline T &F(FileInterfaceAndroid *self, unsigned off) { return *(T *)((char *)self + off); }
static inline int32_t &i32(void *self, unsigned off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, unsigned off) { return *(uint32_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, unsigned off) { return *(uint8_t *)((char *)self + off); }
static inline void *&pp(void *self, unsigned off) { return *(void **)((char *)self + off); }

#endif
