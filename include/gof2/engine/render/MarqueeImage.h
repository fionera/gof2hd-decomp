#ifndef GOF2_MARQUEEIMAGE_H
#define GOF2_MARQUEEIMAGE_H
#include "gof2/common.h"

__attribute__((visibility("hidden"))) extern void **g_MarqueeImage_canvas;

class MarqueeImage {
public:
    uint32_t image;          // image2D handle (filled by PaintCanvas::Image2DCreate)
    int32_t  imageWidth;
    int32_t  imageHeight;
    int32_t  x;
    int32_t  y;
    int32_t  visibleWidth;
    volatile float scrollPosition;
    float    speed;
    int32_t  scrollOffset;

    MarqueeImage(uint16_t image, int width, int x, int y, float speed);
    void setPosition(int x, int y);
    void update(int dt);
    void draw();
    void draw(int x, int y);
};
#endif
