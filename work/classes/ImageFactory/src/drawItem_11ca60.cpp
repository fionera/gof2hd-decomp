#include "class.h"

extern "C" void PaintCanvas_SetColor(unsigned canvas, unsigned color);
extern "C" void PaintCanvas_Image2DCreate(unsigned canvas, unsigned short id, unsigned *out);
extern "C" void PaintCanvas_DrawImage2D(unsigned canvas, int image, int x, int y);
// canvas holder: *g_holder yields the canvas handle (matches the target's double-load).
extern "C" __attribute__((visibility("hidden"))) unsigned *g_drawItem_canvas;

// ImageFactory::drawItem(int, int, int) — draws the item icon for id param_1 at (param_2,param_3).
extern "C" void ImageFactory_drawItem3(int param_1, int param_2, int param_3)
{
    unsigned *holder = g_drawItem_canvas;
    unsigned local;
    PaintCanvas_SetColor(*holder, 0xffffffffu);
    local = 0xffffffffu;
    int base = 0xef0;
    if (param_1 < 0xb0) base = 0x898;
    PaintCanvas_Image2DCreate(*holder, (unsigned short)(base + param_1), &local);
    PaintCanvas_DrawImage2D(*holder, local, param_2, param_3);
}
