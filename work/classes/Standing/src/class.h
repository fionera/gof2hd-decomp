#ifndef WORK_CLASSES_STANDING_SRC_CLASS_H
#define WORK_CLASSES_STANDING_SRC_CLASS_H

// Galaxy on Fire 2 — Standing class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace per the work-item Sig lines).
// Layout recovered from the constructor + per-method disassembly:
//   +0x00  int *standings;   // heap array (operator new[](8) -> 2 ints), [0]=0x1e initially
//   +0x04  int  currentRace; // -1 == "use raw standings[]" mode; >=0 == derived mode
// Most methods reach fields via byte-offset casts from `this`/self (which arrives
// in r0 exactly like the first explicit arg of an extern "C" fn). The ctor/dtor are
// written as real member functions so their demangled symbols carry the method name.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Standing {
    Standing();
    ~Standing();
};

// Field accessor via byte offset.
template <class T>
static inline T &F(Standing *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
