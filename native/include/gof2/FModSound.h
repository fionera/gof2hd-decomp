#ifndef GOF2_FMODSOUND_H
#define GOF2_FMODSOUND_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- FModSound (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "FModSound::init()" etc.
// FMOD wrapper. FMOD_* funcs are modeled as extern "C". Soft-float ABI: float args in core regs.


void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

// Layout offsets ---------------------------------------------------------------
static const uint32_t OFF_EVENTS    = 0x18;    // event handle array (idx*4)
static const uint32_t OFF_ENABLED   = 0x11;    // per-category enabled bytes (0x11..0x14)
static const uint32_t OFF_CATEGORY  = 0x23ec;  // 4 EventCategory* (idx*4)
static const uint32_t OFF_SYSTEM    = 0x23fc;  // EventSystem*
static const uint32_t OFF_MUSIC     = 0x2400;  // music Event*
static const uint32_t OFF_FLAG2404  = 0x2404;

// Field accessors --------------------------------------------------------------
static inline int32_t  &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint8_t  &u8(void *self, uint32_t off)  { return *(uint8_t *)((char *)self + off); }
static inline float    &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void    *&pp(void *self, uint32_t off)  { return *(void **)((char *)self + off); }

struct FModSound { void* _opaque; };  // no offset accesses observed
#endif
