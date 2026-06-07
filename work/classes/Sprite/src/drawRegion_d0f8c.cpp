#include "class.h"

void Sprite::drawRegion(int srcX, int srcY, int w, int h)
{
    uint32_t *frames = field<uint32_t *>(this, 0x0);
    void **holder = g_Sprite_canvas;
    void *canvas = *holder;

    if (frames != 0) {
        int refX = field<int32_t>(this, 0x8);
        int posX = field<int32_t>(this, 0x10);
        int posY = field<int32_t>(this, 0x14);
        uint32_t image = frames[field<int32_t>(this, 0x38)];
        int y = posY + srcY - refX;
        int x = posX + srcX - refX;
        PaintCanvas_DrawRegion2D(canvas, image, srcX, srcY, w, h, 0.0f, 0, 0, x, y);
        return;
    }

    uint32_t image = field<uint32_t>(this, 0x4);
    int refX = field<int32_t>(this, 0x8);
    int posX = field<int32_t>(this, 0x10);
    int posY = field<int32_t>(this, 0x14);
    int frameX = field<int32_t>(this, 0x28);
    int frameY = field<int32_t>(this, 0x2c);
    int y = posY + srcY - refX;
    int x = posX + srcX - refX;
    srcX += frameX;
    srcY += frameY;
    PaintCanvas_DrawRegion2D(canvas, image, srcX, srcY, w, h, 0.0f, 0, 0, x, y);
}
