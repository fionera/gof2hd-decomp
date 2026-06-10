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
    uint32_t field_0x0;    // +0x00  image2D handle (filled by PaintCanvas_Image2DCreate)
    int32_t  field_0x4;    // +0x04  image width
    int32_t  field_0x8;    // +0x08  image height
    int32_t  field_0xc;    // +0x0c  x
    int32_t  field_0x10;   // +0x10  y
    int32_t  field_0x14;   // +0x14  visible width
    volatile float field_0x18; // +0x18  scroll position
    float    field_0x1c;   // +0x1c  speed
    int32_t  field_0x20;   // +0x20  scroll offset

    MarqueeImage(uint16_t image, int width, int x, int y, float speed);
    void setPosition(int x, int y);
    void update(int dt);
    void draw();
    void draw(int x, int y);
};
#endif
