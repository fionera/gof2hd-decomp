#include "class.h"

extern "C" void String_assign(void *dst, String *src);                   // String::operator=
extern "C" int PaintCanvas_GetTextWidth2(void *canvas, void *s, String *t);
extern "C" int PaintCanvas_GetImage2DWidth(void *canvas);
extern "C" int PaintCanvas_GetTextHeight(void *canvas);
extern "C" int PaintCanvas_GetImage2DHeight(void *canvas);
extern "C" void TouchButton_setPosition(TouchButton *self, int x, int y, unsigned char flags);
// PaintCanvas singleton holder (single pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_TB_canvas3;

extern "C" void TouchButton_setText(TouchButton *self, String *text)
{
    String_assign((char *)self + 0xc, text);
    void **holder = g_TB_canvas3;
    int w = PaintCanvas_GetTextWidth2(*holder, P(self, 8), text);
    if (I(self, 0x24) != -1)
        w = PaintCanvas_GetImage2DWidth(*holder);
    int a94 = I(self, 0x94);
    int x;
    if (I(self, 0x6c) < 1)
        x = w;
    else
        x = (I(self, 0x6c) - a94) - I(self, 0x9c);
    I(self, 0xa0) = x;
    x = I(self, 0x9c) + x + a94;
    I(self, 0x90) = x;
    unsigned char fl = UC(self, 0x75);
    if ((fl & 2) == 0) {
        if ((fl & 1) != 0) {
            I(self, 0xa4) = a94;
            goto height;
        }
        x = (x - w) / 2;
        I(self, 0xa4) = x;
        if (I(self, 0x70) == 6) {
            x = x + -5;
        } else {
            if (I(self, 0x70) != 5)
                goto height;
            x = x + 5;
        }
    } else {
        x = x - (w + a94);
    }
    I(self, 0xa4) = x;
height:
    int h = I(self, 0x88);
    int th = PaintCanvas_GetTextHeight(*holder);
    th = (h - th) / 2;
    I(self, 0xa8) = th;
    if (I(self, 0x70) == 3)
        I(self, 0xa8) = th + 2;
    if (I(self, 0x24) != -1) {
        h = I(self, 0x88);
        int ih = PaintCanvas_GetImage2DHeight(*holder);
        I(self, 0xa8) = (h - ih) / 2;
        if (I(self, 0x70) == 1)
            I(self, 0xa4) = I(self, 0xa4) + 3;
    }
    return TouchButton_setPosition(self, I(self, 0x80), I(self, 0x84), UC(self, 0x74));
}
