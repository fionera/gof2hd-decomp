#include "class.h"

Sprite::Sprite(uint32_t image, int frameWidth, int frameHeight)
{
    void **holder = g_Sprite_canvas;

    field<alias_void_ptr>(this, 0x0) = 0;
    field<alias_u32>(this, 0x4) = image;
    field<alias_i32>(this, 0x18) = frameWidth;
    field<alias_i32>(this, 0x1c) = frameHeight;

    void *canvas = *(alias_void_ptr *)holder;
    field<int32_t>(this, 0x20) = PaintCanvas_GetImage2DWidth(canvas, image);
    int imageHeight = PaintCanvas_GetImage2DHeight(*(alias_void_ptr *)holder, field<uint32_t>(this, 0x4));

    int rows = imageHeight / field<int32_t>(this, 0x1c);
    int columns = field<int32_t>(this, 0x20) / field<int32_t>(this, 0x18);

    field<int32_t>(this, 0x8) = 0;
    field<int32_t>(this, 0xc) = 0;
    field<int32_t>(this, 0x10) = 0;
    field<int32_t>(this, 0x14) = 0;

    field<int32_t>(this, 0x24) = imageHeight;
    field<int32_t>(this, 0x30) = columns;
    field<int32_t>(this, 0x34) = rows;
    field<int32_t>(this, 0x3c) = rows * columns;
    setFrame(0);
}
