#include "class.h"

// TouchButton::TouchButton(String const&, int, int, int, int,
//                          unsigned char, unsigned char, unsigned int, int)
//
// Constructs the three embedded String members, temporarily overrides the
// shared font glyph spacing with this button's (spacing, kerning) pair, runs
// the common init(), then restores the previous spacing.

extern "C" void  String_ctor_default(void *s);                 // String::String()
extern "C" short PaintCanvas_FontGetSpacing(void *canvas);
extern "C" void  PaintCanvas_FontSetSpacing(void *canvas, unsigned int spacing, short kerning);
extern "C" void  TouchButton_init(TouchButton *self, String *text,
                                  unsigned int kind, int a, int b, int c, int d,
                                  int x, int y,
                                  unsigned char flags0, unsigned char flags1);

// PaintCanvas singleton (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_canvas_ctor;

extern "C" TouchButton *TouchButton_168f30(TouchButton *self, String *text,
                                           int a, int b, int c, int d,
                                           unsigned char flags0, unsigned char flags1,
                                           unsigned int spacing, int kerning)
{
    String_ctor_default((char *)self + 0xc);
    String_ctor_default((char *)self + 0x18);
    String_ctor_default((char *)self + 0x2c);

    I(self, 0xc4) = kerning;
    U(self, 0x8)  = spacing;

    void *canvas = *g_TB_canvas_ctor;
    short prev = PaintCanvas_FontGetSpacing(canvas);
    PaintCanvas_FontSetSpacing(canvas, spacing, (short)kerning);

    TouchButton_init(self, text, spacing, a, b, c, d, -1, -1, flags0, flags1);

    PaintCanvas_FontSetSpacing(canvas, spacing, prev);
    return self;
}
