#include "gof2/MarqueeImage.h"

// ---- setPosition_15f5f2.cpp ----
void MarqueeImage::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

// ---- MarqueeImage_15f59c.cpp ----
MarqueeImage::MarqueeImage(uint16_t image, int width, int x, int y, float speed)
{
    void **holder = g_MarqueeImage_canvas;

    PaintCanvas_Image2DCreate(*holder, image, &this->image);

    this->x = x;
    this->y = y;
    this->visibleWidth = width;
    this->speed = speed;

    this->imageWidth = PaintCanvas_GetImage2DWidth(*holder, this->image);
    this->imageHeight = PaintCanvas_GetImage2DHeight(*holder, this->image);
    this->scrollPosition = 0;
}

// ---- draw_15f6f4.cpp ----
void MarqueeImage::draw()
{
    return draw(this->x, this->y);
}

// ---- update_15f5fc.cpp ----
void MarqueeImage::update(int dt)
{
    float position = this->scrollPosition;
    float delta = ((float)dt / 1000.0f) * this->speed;
    float width = (float)this->imageWidth;
    position += delta;
    this->scrollPosition = position;
    if (width < position) {
        position = (position - width) + delta * 0.5f;
        this->scrollPosition = position;
    }

    this->scrollOffset = (int)(width - position);
}

// ---- draw_15f654.cpp ----
void MarqueeImage::draw(int x, int y)
{
    int32_t offset = this->scrollOffset;
    this->x = x;
    this->y = y;

    int32_t visibleWidth = this->visibleWidth;

    if (offset > -1) {
        int drawWidth = offset;
        if (drawWidth > visibleWidth) {
            drawWidth = visibleWidth;
        }

        PaintCanvas_DrawRegion2D(*g_MarqueeImage_canvas, this->image, (int)this->scrollPosition,
                                 0, drawWidth, this->imageHeight, 0LL, 0, x, y);
        offset = this->scrollOffset;
    }

    if (offset <= visibleWidth) {
        PaintCanvas_DrawRegion2D(*g_MarqueeImage_canvas, this->image, 0, 0,
                                 visibleWidth - offset,
                                 this->imageHeight, 0LL, 0,
                                 offset + x, y);
    }
}
