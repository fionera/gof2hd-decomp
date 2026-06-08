#include "class.h"

// Hud::drawEventString(String text, bool rightAlign) — draws a HUD event line, right- or
// left-justified depending on rightAlign and the cinematic-letterbox flag at +0x1ec, then
// emits it via PaintCanvas::DrawString. r0=this, r1=text, r2=rightAlign.
extern "C" int  PaintCanvas_GetTextWidth(void *font, void *str);
extern "C" void PaintCanvas_DrawString2(void *canvas, void *font, void *str, int x, char y);

__attribute__((visibility("hidden"))) extern void **g_Hud_font;   // *holder -> font String
__attribute__((visibility("hidden"))) extern void **g_Hud_canvas2;// *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_screenW;// *holder -> [0] = screen width

extern "C" void Hud_drawEventString(Hud *self, void *text, int rightAlign)
{
    void *font = *g_Hud_font;
    void *canvas = *g_Hud_canvas2;
    int x;
    if (UC(self, 0x1ec) == 0) {
        int base = I(self, 0x4e8);
        int yBase = I(self, 0x160);
        if (rightAlign == 0) {
            int w = PaintCanvas_GetTextWidth(canvas, font);
            x = (base + 3) - w;
        } else {
            x = -3 - base;
        }
        x = x + yBase;
    } else {
        if (rightAlign == 0) {
            int margin = I(self, 0x4f0);
            int screenW = *(int *)*g_Hud_screenW;
            int w = PaintCanvas_GetTextWidth(canvas, font);
            x = ((screenW - 1) - margin) - w;
        } else {
            x = I(self, 0x4f0) + 1;
        }
    }
    char y = (char)(I(self, 0x164) - 1);
    PaintCanvas_DrawString2(canvas, font, text, x, y);
}
