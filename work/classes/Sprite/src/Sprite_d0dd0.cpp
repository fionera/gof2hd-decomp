#include "class.h"

Sprite::Sprite(uint32_t *frames, int frameCount, int frameWidth, int frameHeight)
{
    void **holder = g_Sprite_canvas;

    field<alias_void_ptr>(this, 0x0) = frames;
    field<alias_i32>(this, 0x4) = -1;
    field<alias_i32>(this, 0x18) = frameWidth;
    field<alias_i32>(this, 0x1c) = frameHeight;

    void *canvas = *(alias_void_ptr *)holder;
    field<int32_t>(this, 0x20) = PaintCanvas_GetImage2DWidth(canvas, *frames);
    int imageHeight = PaintCanvas_GetImage2DHeight(*(alias_void_ptr *)holder, *field<uint32_t *>(this, 0x0));

    field<int32_t>(this, 0x8) = 0;
    field<int32_t>(this, 0xc) = 0;
    field<int32_t>(this, 0x10) = 0;
    field<int32_t>(this, 0x14) = 0;

    field<int32_t>(this, 0x24) = imageHeight;
    field<int32_t>(this, 0x30) = 1;
    field<int32_t>(this, 0x34) = 1;
    field<int32_t>(this, 0x3c) = frameCount;
    setFrame(0);
}
