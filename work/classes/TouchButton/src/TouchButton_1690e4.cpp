#include "class.h"

extern "C" void String_ctor(void *s);   // AbyssEngine::String::String()
extern "C" int PaintCanvas_FontGetSpacing(void *canvas);
extern "C" void TouchButton_init(TouchButton *self, String *text, unsigned int a,
                                 int b, int x, int y, int p4, int p5, int z,
                                 unsigned char p6, unsigned char c);
// Two singletons (cell value = holder; one deref yields value/object).
__attribute__((visibility("hidden"))) extern int *g_TB_c1;
__attribute__((visibility("hidden"))) extern void **g_TB_c2;

extern "C" TouchButton *TouchButton_ctor6(TouchButton *self, int x, int y, String *text,
                                          int p4, int p5, unsigned char p6)
{
    String_ctor((char *)self + 0xc);
    String_ctor((char *)self + 0x18);
    String_ctor((char *)self + 0x2c);
    I(self, 8) = *(int *)*g_TB_c1;
    I(self, 0xc4) = PaintCanvas_FontGetSpacing(*g_TB_c2);
    TouchButton_init(self, text, 0xffffffff, 4, x, y, p4, p5, 0, p6, 0x44);
    return self;
}
