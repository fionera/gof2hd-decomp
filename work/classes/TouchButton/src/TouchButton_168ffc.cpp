#include "class.h"

// TouchButton::TouchButton(unsigned int, int, int, int, int, unsigned char, unsigned char)
//
// Same shape as the other constructors but the label text comes from an empty
// string literal rather than a caller-supplied String. The font spacing is the
// engine default and the kerning is sampled from the current font.

extern "C" void  String_ctor_default(void *s);                 // String::String()
extern "C" void  String_ctor_cstr(void *s, const char *text, bool copy);
extern "C" void  String_dtor(void *s);
extern "C" int   PaintCanvas_FontGetSpacing(void *canvas);
extern "C" void  TouchButton_init(TouchButton *self, String *text,
                                  unsigned int kind, int a, int b, int c, int d,
                                  int x, int y,
                                  unsigned char flags0, unsigned char flags1);

__attribute__((visibility("hidden"))) extern void **g_TB_canvas_ctor;   // PaintCanvas singleton
__attribute__((visibility("hidden"))) extern unsigned int *g_TB_defSpacing; // default glyph spacing
__attribute__((visibility("hidden"))) extern const char  g_TB_emptyStr[];   // ""

extern "C" void TouchButton_168ffc(TouchButton *self, unsigned int kind,
                                   int a, int b, int c, int d,
                                   unsigned char flags0, unsigned char flags1)
{
    String_ctor_default((char *)self + 0xc);
    String_ctor_default((char *)self + 0x18);
    String_ctor_default((char *)self + 0x2c);

    void *canvas = *g_TB_canvas_ctor;
    U(self, 0x8)  = *g_TB_defSpacing;
    I(self, 0xc4) = PaintCanvas_FontGetSpacing(canvas);

    unsigned char tmp[12];       // String is a 12-byte value type (ctor/dtor are engine calls)
    String_ctor_cstr(tmp, g_TB_emptyStr, false);
    TouchButton_init(self, (String *)tmp, kind, a, b, c, d, -1, -1, flags0, flags1);
    String_dtor(tmp);
}
