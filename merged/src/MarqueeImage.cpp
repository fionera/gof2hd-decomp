#include "MarqueeImage.h"




// ---- setPosition_15f5f2.cpp ----
void MarqueeImage::setPosition(int x, int y)
{
    field<int32_t>(this, 0xc) = x;
    field<int32_t>(this, 0x10) = y;
}

// ---- MarqueeImage_15f59c.cpp ----
MarqueeImage::MarqueeImage(uint16_t image, int width, int x, int y, float speed)
{
    void **holder = g_MarqueeImage_canvas;

    PaintCanvas_Image2DCreate(*holder, image, (uint32_t *)this);

    field<int32_t>(this, 0xc) = x;
    field<int32_t>(this, 0x10) = y;
    field<int32_t>(this, 0x14) = width;
    field<float>(this, 0x1c) = speed;

    field<int32_t>(this, 4) = PaintCanvas_GetImage2DWidth(*holder, field<uint32_t>(this, 0));
    field<int32_t>(this, 8) = PaintCanvas_GetImage2DHeight(*holder, field<uint32_t>(this, 0));
    field<int32_t>(this, 0x18) = 0;
}

// ---- draw_15f6f4.cpp ----
void MarqueeImage::draw()
{
    return draw(field<int32_t>(this, 0xc), field<int32_t>(this, 0x10));
}

// ---- update_15f5fc.cpp ----
void MarqueeImage::update(int dt)
{
    float position = field<float>(this, 0x18);
    float delta = ((float)dt / 1000.0f) * field<float>(this, 0x1c);
    float width = (float)field<int32_t>(this, 4);
    position += delta;
    *(volatile float *)((char *)this + 0x18) = position;
    if (width < position) {
        position = (position - width) + delta * 0.5f;
        *(volatile float *)((char *)this + 0x18) = position;
    }

    field<int32_t>(this, 0x20) = (int)(width - position);
}

// ---- draw_15f654.cpp ----
void MarqueeImage::draw(int x, int y)
{
    char *self = (char *)this;
    int32_t offset = *(int32_t *)(self + 0x20);
    *(int32_t *)(self + 0xc) = x;
    *(int32_t *)(self + 0x10) = y;

    register uint32_t visibleWidth;
    register int drawX = x;
    register int zero = 0;
    if (offset <= -1) {
        goto skippedFirst;
    }

    visibleWidth = (uint32_t)self;
    visibleWidth += 0x14;
    {
        int drawWidth = offset;
        if (drawWidth > *(int32_t *)visibleWidth) {
            drawWidth = *(int32_t *)visibleWidth;
        }

        PaintCanvas_DrawRegion2D(*g_MarqueeImage_canvas, *(uint32_t *)self, (int)*(float *)(self + 0x18),
                                 zero, drawWidth, *(int32_t *)(self + 8), 0LL, zero, drawX, y);
        offset = *(int32_t *)(self + 0x20);
    }
    goto afterFirst;

skippedFirst:
    visibleWidth = (uint32_t)(self + 0x14);

afterFirst:
    if (offset <= *(int32_t *)visibleWidth) {
        PaintCanvas_DrawRegion2D(*g_MarqueeImage_canvas, *(uint32_t *)self, 0, 0,
                                 *(int32_t *)visibleWidth - offset,
                                 *(int32_t *)(self + 8), 0LL, zero,
                                 offset + drawX, y);
    }
}
