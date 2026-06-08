#ifndef WORK_CLASSES_HANGARWINDOW_SRC_CLASS_H
#define WORK_CLASSES_HANGARWINDOW_SRC_CLASS_H

// Galaxy on Fire 2 - HangarWindow class. Android libgof2hdaa.so, armv7 Thumb.
// Top-level class (no AbyssEngine namespace). Field offsets recovered per-method from the
// target disassembly; we do NOT model a full layout - access fields via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

// Opaque referenced types.
struct HangarWindow;
struct HangarList;
struct ListItem;
struct Item;
struct BluePrint;
struct Station;

template <class T>
struct Array {
    uint32_t length;   // +0x00 element count
    T *data;           // +0x04 element buffer
    uint32_t cap;      // +0x08 capacity
};

// AbyssEngine::AEMath::Vector - 3 floats.
namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
} }
typedef AbyssEngine::AEMath::Vector Vector;

// Field accessors via byte offset.
template <class T> static inline T &F(HangarWindow *p, unsigned off) {
    return *(T *)((char *)p + off);
}
template <class T> static inline T &G(void *p, unsigned off) {
    return *(T *)((char *)p + off);
}

#endif
