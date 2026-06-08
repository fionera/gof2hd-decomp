#include "class.h"

// ImageFactory::drawShip(int shipId, int x, int y) — draws the composite ship sprite (frame 5)
// at (x,y), then overlays the ship's class icon (image id 0x971+shipId).
extern "C" void IF_Sprite_setFrame(void *spr, int frame);
extern "C" long long IF_Sprite_setPosition(void *spr, int x, int y);
extern "C" void IF_Sprite_draw(float sx, float sy);
extern "C" void IF_PaintCanvas_SetColor(unsigned canvas, unsigned color);
extern "C" void IF_PaintCanvas_Image2DCreate(unsigned canvas, unsigned short id, unsigned *out);
extern "C" void IF_PaintCanvas_DrawImage2D(unsigned canvas, int image, int x, int y);
extern "C" __attribute__((visibility("hidden"))) unsigned *g_IF_drawShip_canvas;

extern "C" void ImageFactory_drawShip(ImageFactory *self, int shipId, int x, int y)
{
    unsigned *holder = g_IF_drawShip_canvas;
    unsigned local = 0xffffffffu;
    IF_PaintCanvas_SetColor(*holder, 0xffffffffu);
    IF_Sprite_setFrame(F<void *>(self, 0x0), 5);
    IF_Sprite_setPosition(F<void *>(self, 0x0), x, y);
    IF_Sprite_draw(1.0f, 1.0f);
    IF_PaintCanvas_Image2DCreate(*holder, (unsigned short)(shipId + 0x971), &local);
    IF_PaintCanvas_DrawImage2D(*holder, (int)local, x, y);
}
