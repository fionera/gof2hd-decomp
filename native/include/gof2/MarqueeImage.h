#ifndef GOF2_MARQUEEIMAGE_H
#define GOF2_MARQUEEIMAGE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
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

extern "C" void PaintCanvas_Image2DCreate(void *canvas, uint16_t image, uint32_t *out);
extern "C" int PaintCanvas_GetImage2DWidth(void *canvas, uint32_t image);
extern "C" int PaintCanvas_GetImage2DHeight(void *canvas, uint32_t image);
extern "C" void PaintCanvas_DrawRegion2D(void *canvas, uint32_t image, int srcX, int srcY, int w, int h,
                                         long long zeroPair, int flags, int x, int y);

__attribute__((visibility("hidden"))) extern void **g_MarqueeImage_canvas;

struct MarqueeImage {
    volatile float field_0x18;          // +0x18
};
#endif
