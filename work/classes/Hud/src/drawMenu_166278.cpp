#include "class.h"

// Hud::drawMenu(int) — draws the radial quick-menu: the rounded background (top cap, repeated
// middle slices, bottom cap), the menu buttons, and (when the ship has cloak/jump drive) the
// extra "fuel" gauge with its numeric label. Ghidra mislabels: r0=this. r0 is the menu obj.
extern "C" void Layout_drawMask();
extern "C" void PaintCanvas_DrawImage2D2(void *canvas, int img, int xy);
extern "C" void PaintCanvas_DrawImage2D5(void *canvas, int img, int x, int y, char anchor);
extern "C" void TouchButton_draw(void *btn);
extern "C" void *Status_getShip();
extern "C" int  Ship_hasCloak(void *ship);
extern "C" int  Ship_hasJumpDrive(void *ship);
extern "C" void String_ctor_cstr(void *s, const char *cstr, bool b);
extern "C" void String_ctor_int(void *s, int v);
extern "C" void String_concat(void *out, void *lhs, void *rhs);
extern "C" void String_dtor(void *s);
extern "C" int  PaintCanvas_GetImage2DHeight(void *canvas);
extern "C" int  PaintCanvas_GetTextHeight(void *canvas);
extern "C" void PaintCanvas_DrawString2(void *canvas, void *font, void *str, int x, char y);

__attribute__((visibility("hidden"))) extern void **g_Hud_dmLayout; // *holder -> layout obj (*piVar6)
__attribute__((visibility("hidden"))) extern void **g_Hud_dmCanvas; // *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_dmFont;   // *holder -> font String
extern const char g_Hud_dmPrefix[] __attribute__((visibility("hidden")));

extern "C" void Hud_drawMenu(Hud *self)
{
    Layout_drawMask();
    void *canvas = *g_Hud_dmCanvas;
    int *layout = (int *)*g_Hud_dmLayout;

    // top cap
    PaintCanvas_DrawImage2D2(canvas, I(self, 0x298), I(self, 0x3c4) + I(self, 0x4cc));
    // header glyph (centered)
    int hx = I(self, 0x4cc) + I(self, 0x3d4) + I(self, 0x3dc) / 2;
    char hy = (char)((char)I(self, 0x4d0) + (char)I(self, 0x3c8) + (char)(I(self, 0x3cc) / 2)
                     - (char)layout[0x8b] /*+0x22c*/);
    PaintCanvas_DrawImage2D5(canvas, I(self, 0x35c), hx, hy, 0x11);

    int y = I(self, 0x3c8) + I(self, 0x4d0) + I(self, 0x3cc);
    // repeated middle slices (one per button beyond the first)
    if (self->f_18 != 0 && *(int *)self->f_18 != 0) {
        int count = *(int *)self->f_18;
        for (unsigned int i = 0; i < (unsigned int)(count - 1); i++) {
            PaintCanvas_DrawImage2D2(canvas, I(self, 0x2a0), I(self, 0x3c4) + I(self, 0x4cc));
            y += I(self, 0x3d0);
            count = *(int *)self->f_18;
        }
    }
    // bottom cap
    PaintCanvas_DrawImage2D2(canvas, I(self, 0x29c), I(self, 0x3c4) + I(self, 0x4cc));

    // the actual buttons
    if (self->f_18 != 0 && *(unsigned int *)self->f_18 != 0) {
        unsigned int n = *(unsigned int *)self->f_18;
        for (unsigned int i = 0; i < n; i++) {
            TouchButton_draw(((void **)I(self->f_18, 4))[i]);
            n = *(unsigned int *)self->f_18;
        }
    }

    if (I(self, 0x238) != 0) return;

    Status_getShip();
    int cloak = Ship_hasCloak(Status_getShip());
    if (cloak == 0) {
        Status_getShip();
        if (Ship_hasJumpDrive(Status_getShip()) == 0) return;
    }

    char prefix[12], num[12], label[12];
    String_ctor_cstr(prefix, g_Hud_dmPrefix, false);
    String_ctor_int(num, I(self, 0x27c));
    String_concat(label, prefix, num);
    String_dtor(num);
    String_dtor(prefix);

    int gx = I(self, 0x4cc) + I(self, 0x3d4) + I(self, 0x3dc) / 2;
    unsigned char gy = (unsigned char)((char)y + (char)(layout[0xc] /*+0x30*/ / 2)
                        + (char)layout[0xa2] /*+0x288*/);
    PaintCanvas_DrawImage2D5(canvas, I(self, 0x374), gx, gy, 0x11);
    PaintCanvas_DrawImage2D5(canvas, I(self, 0x370), gx - layout[0x8c] /*+0x230*/,
        (char)layout[0xc] + (char)gy + (char)layout[0xa3] /*+0x28c*/, 0x11);

    int barW = layout[0x8c];
    void *font = *g_Hud_dmFont;
    int ih = PaintCanvas_GetImage2DHeight(canvas);
    int th = PaintCanvas_GetTextHeight(canvas);
    char ty = (char)(((gy + (char)(ih / 2)) - (char)(th / 2)) + (char)layout[0x8d] /*+0x234*/);
    PaintCanvas_DrawString2(canvas, font, label, barW + gx, ty);
    String_dtor(label);
}
