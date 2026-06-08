#ifndef WORK_CLASSES_STRING_SRC_CLASS_H
#define WORK_CLASSES_STRING_SRC_CLASS_H

// Galaxy on Fire 2 - AbyssEngine::String. Android libgof2hdaa.so, armv7 Thumb.
// Namespaced class (namespace AbyssEngine). Recovered layout:
//   +0x00  vtable pointer
//   +0x04  ushort* data   (UTF-16 / wide character buffer, NUL-terminated)
//   +0x08  uint   length  (number of code units, excluding terminator)
// Fields are accessed via byte-offset casts; we do NOT model a full layout.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

namespace AbyssEngine {
struct String;
}
typedef AbyssEngine::String String;

// Array<T> container header (length / data / capacity) used by Split / SplitTags.
template <class T>
struct Array {
    uint32_t length;   // +0x00 element count
    T *data;           // +0x04 element buffer
    uint32_t cap;      // +0x08 capacity
};

// Field accessors via byte offset.
template <class T> static inline T &F(String *p, unsigned off) {
    return *(T *)((char *)p + off);
}
template <class T> static inline T &G(void *p, unsigned off) {
    return *(T *)((char *)p + off);
}

#endif
