#include "class.h"

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
