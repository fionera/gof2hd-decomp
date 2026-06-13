#ifndef GOF2_MARQUEEIMAGE_H
#define GOF2_MARQUEEIMAGE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)

template <class T>
static inline const T &field(const void *self, uint32_t off)
{
    return *(const T *)((const char *)self + off);
}

__attribute__((visibility("hidden"))) extern void **g_MarqueeImage_canvas;

class MarqueeImage {
public:
    uint32_t image;    // +0x00  image2D handle (filled by PaintCanvas::Image2DCreate)
    int32_t  imageWidth;    // +0x04  image width
    int32_t  imageHeight;    // +0x08  image height
    int32_t  x;    // +0x0c  x
    int32_t  y;   // +0x10  y
    int32_t  visibleWidth;   // +0x14  visible width
    volatile float scrollPosition; // +0x18  scroll position
    float    speed;   // +0x1c  speed
    int32_t  scrollOffset;   // +0x20  scroll offset

    MarqueeImage(uint16_t image, int width, int x, int y, float speed);
    void setPosition(int x, int y);
    void update(int dt);
    void draw();
    void draw(int x, int y);
};
#endif
