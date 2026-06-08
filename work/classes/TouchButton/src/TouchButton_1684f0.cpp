#include "class.h"

// TouchButton::TouchButton(String const& text, int x, int y, int p4, uchar p5)
// 5-arg overload: same setup as the other label ctors -- three embedded Strings, font color
// (+0x08), cached spacing (+0xc4) -- then init().

extern "C" void String_ctor(void *s);
extern "C" int  PaintCanvas_FontGetSpacing(void *canvas);
extern "C" void TouchButton_init(TouchButton *self, String *text, unsigned int a,
                                 int b, int x, int y, int p4, int p5, int z,
                                 unsigned char p6, unsigned char c);
__attribute__((visibility("hidden"))) extern int *g_TB_c1;
__attribute__((visibility("hidden"))) extern void **g_TB_c2;

extern "C" TouchButton *TouchButton_ctor5(TouchButton *self, String *text, int x, int y,
                                          int p4, unsigned char p5)
{
    String_ctor((char *)self + 0xc);
    String_ctor((char *)self + 0x18);
    String_ctor((char *)self + 0x2c);
    I(self, 8) = *(int *)*g_TB_c1;
    I(self, 0xc4) = PaintCanvas_FontGetSpacing(*g_TB_c2);
    TouchButton_init(self, text, 0xffffffff, 4, x, y, p4, 0, 0, p5, 0x44);
    return self;
}
