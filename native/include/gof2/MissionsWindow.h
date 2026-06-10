#ifndef GOF2_MISSIONSWINDOW_H
#define GOF2_MISSIONSWINDOW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- MissionsWindow (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace) -- the qualified target name is
// "MissionsWindow::...". Field offsets are recovered per-method from the target
// disassembly; we do NOT model a full layout -- fields are reached via byte-offset casts.


void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

// Field accessors via byte offset.
static inline int32_t  &i32(void *self, uint32_t off) { return *(int32_t  *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline short    &i16(void *self, uint32_t off) { return *(short    *)((char *)self + off); }
static inline uint8_t  &u8 (void *self, uint32_t off) { return *(uint8_t  *)((char *)self + off); }
static inline float    &f32(void *self, uint32_t off) { return *(float    *)((char *)self + off); }
static inline void    *&pp (void *self, uint32_t off) { return *(void    **)((char *)self + off); }

struct MissionsWindow {
    Blk16 field_0xc;                    // +0xc
};
#endif
