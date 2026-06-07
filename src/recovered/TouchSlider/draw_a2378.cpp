#include "class.h"

struct TouchSlider {
    void draw();
};

extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void PaintCanvas_DrawImage2D(void *canvas, int image, int x, int y);
extern "C" void PaintCanvas_DrawImage2D6(void *canvas, int image, int x, int y, int a, int b);

// PaintCanvas singleton holder (single pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_TouchSlider_canvas;

void TouchSlider::draw()
{
    void **holder = g_TouchSlider_canvas;
    int color = F<uint8_t>(this, 0x35) != 0 ? 0xFFFFFF2F : -1;
    PaintCanvas_SetColor(*holder, color);
    PaintCanvas_DrawImage2D(*holder, F<int>(this, 0x2c), F<int>(this, 0x0), F<int>(this, 0x4));
    PaintCanvas_DrawImage2D6(*holder, F<int>(this, 0x30), F<int>(this, 0x8), F<int>(this, 0xc), 0x11, 0x44);
}
