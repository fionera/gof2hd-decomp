#ifndef GOF2_SPRITE_H
#define GOF2_SPRITE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
typedef void *alias_void_ptr __attribute__((may_alias));
typedef uint32_t alias_u32 __attribute__((may_alias));
typedef int32_t alias_i32 __attribute__((may_alias));



template <class T>
static inline T &field(void *self, uint32_t off)
{
    return *(T *)((char *)self + off);
}

template <class T>
static inline const T &field(const void *self, uint32_t off)
{
    return *(const T *)((const char *)self + off);
}

void operator delete(void *ptr) noexcept;

extern "C" int __aeabi_idiv(int numerator, int denominator);
extern "C" int __aeabi_idivmod(int numerator, int denominator);

extern "C" int PaintCanvas_GetImage2DWidth(void *canvas, uint32_t image);
extern "C" int PaintCanvas_GetImage2DHeight(void *canvas, uint32_t image);
extern "C" void PaintCanvas_DrawRegion2D(void *canvas, uint32_t image, int srcX, int srcY, int w, int h,
                                         float alpha, int p7, int p8, int x, int y);
extern "C" void PaintCanvas_DrawImage2D(void *canvas, uint32_t image, int x, int y);
extern "C" void PaintCanvas_DrawImage2DScaled(void *canvas, uint32_t image, int x, int y, int w, int h,
                                             int anchorX, int anchorY, int flags);

__attribute__((visibility("hidden"))) extern void **g_Sprite_canvas;

struct Sprite { void* _opaque; };  // no offset accesses observed
#endif
