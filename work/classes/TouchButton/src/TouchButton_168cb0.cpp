#include "class.h"

// TouchButton::TouchButton(unsigned int, int, int, int, unsigned char)
//
// Five-argument convenience constructor: kind + position + a single flag byte.
// Builds the three embedded Strings, samples the default spacing/kerning, then
// runs init() with an empty label and a fixed extra parameter (0x44).

extern "C" void  String_ctor_default(void *s);
extern "C" void  String_ctor_cstr(void *s, const char *text, bool copy);
extern "C" void  String_dtor(void *s);
extern "C" int   PaintCanvas_FontGetSpacing(void *canvas);
extern "C" void  TouchButton_init(TouchButton *self, String *text,
                                  unsigned int kind, int a, int b, int c, int d,
                                  int x, int y,
                                  unsigned char flags0, unsigned char flags1);

__attribute__((visibility("hidden"))) extern void **g_TB_canvas_ctor;
__attribute__((visibility("hidden"))) extern unsigned int *g_TB_defSpacing;
__attribute__((visibility("hidden"))) extern const char  g_TB_emptyStr[];

extern "C" void TouchButton_168cb0(TouchButton *self, unsigned int kind,
                                   int a, int b, int c, unsigned char flag)
{
    String_ctor_default((char *)self + 0xc);
    String_ctor_default((char *)self + 0x18);
    String_ctor_default((char *)self + 0x2c);

    void *canvas = *g_TB_canvas_ctor;
    U(self, 0x8)  = *g_TB_defSpacing;
    I(self, 0xc4) = PaintCanvas_FontGetSpacing(canvas);

    unsigned char tmp[12];       // String is a 12-byte value type
    String_ctor_cstr(tmp, g_TB_emptyStr, false);
    TouchButton_init(self, (String *)tmp, kind, a, b, c, 0x44, -1, -1, flag, 0);
    String_dtor(tmp);
}
