#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_Sprite_draw_image_canvas;
__attribute__((visibility("hidden"))) extern void **g_Sprite_draw_region_canvas;
extern "C" void PaintCanvas_DrawRegion2DForDraw(void *canvas, uint32_t image, int srcX, int srcY, int w, int h,
                                                long long zeroPair, int flags, int x, int y);

void Sprite::draw(float scaleX, float scaleY)
{
    uint32_t *frames = field<uint32_t *>(this, 0x0);

    if (frames == 0) {
        uint32_t image = field<uint32_t>(this, 0x4);
        int refX = field<int32_t>(this, 0x8);
        int posX = field<int32_t>(this, 0x10);
        int posY = field<int32_t>(this, 0x14);
        int w = field<int32_t>(this, 0x18);
        int h = field<int32_t>(this, 0x1c);
        int y = posY - refX;
        posY = 0;
        int x = posX - refX;
        PaintCanvas_DrawRegion2DForDraw(*(alias_void_ptr *)g_Sprite_draw_region_canvas, image,
                                        field<int32_t>(this, 0x28), field<int32_t>(this, 0x2c), w, h,
                                        (long long)(uint32_t)posY, posY, x, y);
        return;
    }

    void **holder = g_Sprite_draw_image_canvas;
    int frame = field<int32_t>(this, 0x38);
    int refX = field<int32_t>(this, 0x8);
    int posX = field<int32_t>(this, 0x10);
    void *holderValue = *holder;
    uint32_t image = frames[frame];
    int x = posX - refX;
    void *canvas = holderValue;

    if (scaleX == 1.0f || scaleY == 1.0f) {
        return PaintCanvas_DrawImage2D(canvas, image, x, field<int32_t>(this, 0x14) - refX);
    }

    float frameHeight = (float)field<int32_t>(this, 0x1c);
    float frameWidth = (float)field<int32_t>(this, 0x18);
    int y = field<int32_t>(this, 0x14) - refX;
    int scaledHeight = (int)(frameHeight * scaleY);
    int scaledWidth = (int)(frameWidth * scaleX);
    int drawY = (int)((float)y - ((scaleY - 1.0f) * frameHeight) * 0.5f);
    int drawX = (int)((float)x - ((scaleX - 1.0f) * frameWidth) * 0.5f);

    PaintCanvas_DrawImage2DScaled(canvas, image, drawX, drawY, scaledWidth, scaledHeight, 0x11, 0x11, 0);
}
