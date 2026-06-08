#include "class.h"

// TouchButton::TouchButton(String const& text, int type, int x, int y, int p5, uchar p6, uchar p7)
// Default-constructs the three embedded label Strings (+0xc/+0x18/+0x2c), seeds the font color
// (+0x08) from the active-font global, caches the current font spacing (+0xc4), then delegates
// to init() which builds the button's images and layout.

extern "C" void String_ctor(void *s);
extern "C" int  PaintCanvas_FontGetSpacing(void *canvas);
extern "C" void TouchButton_init(TouchButton *self, String *text, unsigned int a,
                                 int b, int x, int y, int p4, int p5, int z,
                                 unsigned char p6, unsigned char c);
__attribute__((visibility("hidden"))) extern int *g_TB_c1;
__attribute__((visibility("hidden"))) extern void **g_TB_c2;

extern "C" TouchButton *TouchButton_ctor7(TouchButton *self, String *text, int type, int x, int y,
                                          int p5, unsigned char p6, unsigned char p7)
{
    String_ctor((char *)self + 0xc);
    String_ctor((char *)self + 0x18);
    String_ctor((char *)self + 0x2c);
    I(self, 8) = *(int *)*g_TB_c1;
    I(self, 0xc4) = PaintCanvas_FontGetSpacing(*g_TB_c2);
    TouchButton_init(self, text, (unsigned int)type, x, y, p5, 0, 0, 0, p6, p7);
    return self;
}
