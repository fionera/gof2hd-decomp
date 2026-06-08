#ifndef WORK_CLASSES_LISTITEMWINDOW_SRC_CLASS_H
#define WORK_CLASSES_LISTITEMWINDOW_SRC_CLASS_H

// Galaxy on Fire 2 -- ListItemWindow (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace) -- the qualified target name is
// "ListItemWindow::...". Only argument types live in AbyssEngine. Field offsets are
// recovered per-method from the target disassembly; we do NOT model a full layout --
// fields are reached through byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

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
template <class T> static inline T &F(void *p, uint32_t off) { return *(T *)((char *)p + off); }

#endif
