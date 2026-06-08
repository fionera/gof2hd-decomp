#ifndef WORK_CLASSES_AUTOPILOTLIST_SRC_CLASS_H
#define WORK_CLASSES_AUTOPILOTLIST_SRC_CLASS_H

// Galaxy on Fire 2 - AutoPilotList class. Android libgof2hdaa.so, armv7 Thumb.
// Top-level class (no AbyssEngine namespace; only some arg/field types live in
// AbyssEngine). Field offsets recovered per-method from the target disassembly; we do
// NOT model a full layout - access fields via byte-offset casts from `this`.
//
// Known field offsets:
//   0x00 int    selected index
//   0x04 int    x
//   0x08 int    y
//   0x0c int    width
//   0x10 Array<String*>*  entries (owned; released in dtor)
//   0x14 int    count

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Level;

// Methods authored as real C++ members demangle to AutoPilotList::name(); the rest are
// extern "C" wrappers with hand-mangled names.
struct AutoPilotList {
    // @portable-fields
    int f_0; // 0x0
    int f_4; // 0x4
    int f_8; // 0x8
    int f_c; // 0xc
    void* f_10; // 0x10
    int f_14; // 0x14

    void draw();
};

template <class T>
struct Array {
    uint32_t length;   // +0x00 element count
    T *data;           // +0x04 element buffer
    uint32_t cap;      // +0x08 capacity
};

// AbyssEngine::String - 12-byte object. Heap-allocated, constructed via engine helpers.
struct EngString {
    uint32_t a, b, c;
    EngString(const void *src, bool copy);   // String(String*, bool) / String(char*, bool)
};
void *operator new(__SIZE_TYPE__);

// AbyssEngine::String passed/returned by value: 12-byte trivially-copied aggregate.
struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

// Field accessor via byte offset.
template <class T> static inline T &F(AutoPilotList *p, unsigned off) {
    return *(T *)((char *)p + off);
}

#endif
