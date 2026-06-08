#include "class.h"

// ImageFactory::drawItem(int itemId, int frame, int x, int y) — draws the item's composite
// sprite at the given frame/position, then overlays the item icon (0x898 for ids < 0xb0,
// otherwise 0xef0, plus itemId).
extern "C" void IF_Sprite_setFrame(void *spr, int frame);
extern "C" long long IF_Sprite_setPosition(void *spr, int x, int y);
extern "C" void IF_Sprite_draw(float sx, float sy);
extern "C" void IF_PaintCanvas_SetColor(unsigned canvas, unsigned color);
extern "C" void IF_PaintCanvas_Image2DCreate(unsigned canvas, unsigned short id, unsigned *out);
extern "C" void IF_PaintCanvas_DrawImage2D(unsigned canvas, int image, int x, int y);
extern "C" __attribute__((visibility("hidden"))) unsigned *g_IF_drawItem4_canvas;

extern "C" void ImageFactory_drawItem4(ImageFactory *self, int itemId, int frame, int x, int y)
{
    unsigned *holder = g_IF_drawItem4_canvas;
    unsigned local = 0xffffffffu;
    IF_PaintCanvas_SetColor(*holder, 0xffffffffu);
    IF_Sprite_setFrame(F<void *>(self, 0x0), frame);
    IF_Sprite_setPosition(F<void *>(self, 0x0), x, y);
    IF_Sprite_draw(1.0f, 1.0f);
    int base = 0xef0;
    if (itemId < 0xb0)
        base = 0x898;
    IF_PaintCanvas_Image2DCreate(*holder, (unsigned short)(base + itemId), &local);
    IF_PaintCanvas_DrawImage2D(*holder, (int)local, x, y);
}
