#ifndef WORK_CLASSES_MENUTOUCHWINDOW_SRC_CLASS_H
#define WORK_CLASSES_MENUTOUCHWINDOW_SRC_CLASS_H

// Galaxy on Fire 2 -- MenuTouchWindow (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name is top-level: "MenuTouchWindow::..." (class not in a namespace;
// only its argument types are, e.g. AbyssEngine::...).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;

void *operator new(uint32_t size);
void operator delete(void *ptr) noexcept;

// Field accessors --------------------------------------------------------------
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline short &i16(void *self, uint32_t off) { return *(short *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

#endif
