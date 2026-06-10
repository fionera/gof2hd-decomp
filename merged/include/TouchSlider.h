#ifndef WORK_CLASSES_TOUCHSLIDER_SRC_CLASS_H
#define WORK_CLASSES_TOUCHSLIDER_SRC_CLASS_H

// Galaxy on Fire 2 -- TouchSlider (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "TouchSlider::..." (NOT in a namespace).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct TouchSlider;

// Templated byte-offset field accessor: F<int>(this, 0x14) etc.
template <class T>
static inline T &F(void *p, unsigned off) {
    return *(T *)((char *)p + off);
}

#endif
