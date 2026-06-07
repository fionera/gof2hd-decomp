#include "class.h"

extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void PaintCanvas_drawImage(void *canvas, int img, int x, int y);
// PaintCanvas singleton holder (single pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_Hud_canvas;

extern "C" void Hud_drawPauseButton(Hud *self)
{
    void **holder = g_Hud_canvas;
    PaintCanvas_SetColor(*holder, -1);
    unsigned char flag = UC(self, 0x284);
    int y = US(self, 0x40c);
    int x = US(self, 0x40a);
    int img = (flag & 1) == 0 ? I(self, 0x2f8) : I(self, 0x2f4);
    return PaintCanvas_drawImage(*holder, img, x, y);
}
