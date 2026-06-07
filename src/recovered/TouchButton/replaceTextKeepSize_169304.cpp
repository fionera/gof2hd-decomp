#include "class.h"

extern "C" void String_assign(void *dst, String *src);            // String::operator=
extern "C" int PaintCanvas_GetTextWidth(void *canvas, void *str, String *s);
// PaintCanvas singleton (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_canvas2;

extern "C" void TouchButton_replaceTextKeepSize(TouchButton *self, String *text)
{
    String_assign((char *)self + 0xc, text);
    if (I(self, 0x70) == 10) {
        int w = I(self, 0x90);
        int tw = PaintCanvas_GetTextWidth(*g_TB_canvas2, P(self, 8), text);
        I(self, 0xa4) = w / 2 - tw / 2;
    }
}
