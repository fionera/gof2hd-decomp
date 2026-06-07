#include "class.h"

extern "C" void String_ctor(void *self);
extern "C" int PaintCanvas_GetTextHeight(void *canvas, unsigned int font);
extern "C" void PaintCanvas_Image2DCreate(void *canvas, int image, void *out);
extern "C" int WantedWindow_init(WantedWindow *self);

__attribute__((visibility("hidden"))) extern void **g_WantedWindow_ctor_canvas;
__attribute__((visibility("hidden"))) extern unsigned int **g_WantedWindow_ctor_font;

extern "C" WantedWindow *_ZN12WantedWindowC2Ev(WantedWindow *self)
{
    String_ctor((char *)self + 0x3c);
    String_ctor((char *)self + 0x48);
    String_ctor((char *)self + 0x54);
    String_ctor((char *)self + 0x60);
    String_ctor((char *)self + 0x6c);
    String_ctor((char *)self + 0x78);

    void **canvasHolder = g_WantedWindow_ctor_canvas;
    F<void *>(self, 0x18) = 0;
    F<void *>(self, 0x4) = 0;
    F<void *>(self, 0x8) = 0;
    unsigned int *fontHolder = *g_WantedWindow_ctor_font;
    int h = PaintCanvas_GetTextHeight(*canvasHolder, *fontHolder);
    F<void *>(self, 0x38) = 0;
    F<void *>(self, 0xb0) = 0;
    F<void *>(self, 0x2c) = 0;
    F<void *>(self, 0x0c) = 0;
    F<int>(self, 0x10) = h / 2 - 1;
    PaintCanvas_Image2DCreate(*canvasHolder, 0x454, (char *)self + 0xac);
    WantedWindow_init(self);
    return self;
}
