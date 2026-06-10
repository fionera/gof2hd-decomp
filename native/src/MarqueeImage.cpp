#include "gof2/MarqueeImage.h"

// ---- setPosition_15f5f2.cpp ----
void MarqueeImage::setPosition(int x, int y)
{
    this->field_0xc = x;
    this->field_0x10 = y;
}

// ---- MarqueeImage_15f59c.cpp ----
MarqueeImage::MarqueeImage(uint16_t image, int width, int x, int y, float speed)
{
    void **holder = g_MarqueeImage_canvas;

    PaintCanvas_Image2DCreate(*holder, image, &this->field_0x0);

    this->field_0xc = x;
    this->field_0x10 = y;
    this->field_0x14 = width;
    this->field_0x1c = speed;

    this->field_0x4 = PaintCanvas_GetImage2DWidth(*holder, this->field_0x0);
    this->field_0x8 = PaintCanvas_GetImage2DHeight(*holder, this->field_0x0);
    this->field_0x18 = 0;
}

// ---- draw_15f6f4.cpp ----
void MarqueeImage::draw()
{
    return draw(this->field_0xc, this->field_0x10);
}

// ---- update_15f5fc.cpp ----
void MarqueeImage::update(int dt)
{
    float position = this->field_0x18;
    float delta = ((float)dt / 1000.0f) * this->field_0x1c;
    float width = (float)this->field_0x4;
    position += delta;
    this->field_0x18 = position;
    if (width < position) {
        position = (position - width) + delta * 0.5f;
        this->field_0x18 = position;
    }

    this->field_0x20 = (int)(width - position);
}

// ---- draw_15f654.cpp ----
void MarqueeImage::draw(int x, int y)
{
    int32_t offset = this->field_0x20;
    this->field_0xc = x;
    this->field_0x10 = y;

    int32_t visibleWidth = this->field_0x14;

    if (offset > -1) {
        int drawWidth = offset;
        if (drawWidth > visibleWidth) {
            drawWidth = visibleWidth;
        }

        PaintCanvas_DrawRegion2D(*g_MarqueeImage_canvas, this->field_0x0, (int)this->field_0x18,
                                 0, drawWidth, this->field_0x8, 0LL, 0, x, y);
        offset = this->field_0x20;
    }

    if (offset <= visibleWidth) {
        PaintCanvas_DrawRegion2D(*g_MarqueeImage_canvas, this->field_0x0, 0, 0,
                                 visibleWidth - offset,
                                 this->field_0x8, 0LL, 0,
                                 offset + x, y);
    }
}
