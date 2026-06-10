#ifndef GOF2_LISTITEMWINDOW_H
#define GOF2_LISTITEMWINDOW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- ListItemWindow (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace) -- the qualified target name is
// "ListItemWindow::...". Only argument types live in AbyssEngine. The recovered
// instance layout mixes the engine's 12-byte String and fixed-size Matrix at hard
// offsets (and several runtime-computed offsets such as the up/down/equal arrow
// image ids), so the body reaches fields through the typed byte-offset helpers
// below rather than a synthetic std-typed field list.

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

// Engine String is a 12-byte trivially-copyable value (ctor/dtor are engine calls).
struct Str { uint32_t a, b, c; };

// ListItemWindow instance: opaque storage reached via the typed offset helpers.
struct ListItemWindow {
    ListItemWindow();
    ~ListItemWindow();
    void OnTouchBegin(int x, int y);
    void OnTouchMove(int x, int y);
    void OnTouchEnd(int x, int y);
    uint8_t shows3DShip();
    void render();
    void set(void *item, unsigned p2, unsigned p3, unsigned p4, unsigned p5, bool p6);
    void draw();
    void update(int frameTime);
};

// Field accessors via byte offset.
static inline int32_t  &i32(void *self, uint32_t off) { return *(int32_t  *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline short    &i16(void *self, uint32_t off) { return *(short    *)((char *)self + off); }
static inline uint8_t  &u8 (void *self, uint32_t off) { return *(uint8_t  *)((char *)self + off); }
static inline float    &f32(void *self, uint32_t off) { return *(float    *)((char *)self + off); }
static inline void    *&pp (void *self, uint32_t off) { return *(void    **)((char *)self + off); }

#endif
