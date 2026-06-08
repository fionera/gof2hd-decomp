#include "class.h"

struct PaintCanvas;

extern "C" void PaintCanvas_SetColor(unsigned color);                      // 0x6fac0
extern "C" int PaintCanvas_GetImage2DWidth(int img);                       // 0x72d84
extern "C" int PaintCanvas_GetImage2DHeight(int img);                      // 0x72d90
extern "C" void PaintCanvas_DrawImage2D3(PaintCanvas *pc, int img, int x, int y);  // 0x72dc0
extern "C" void Layout_drawBGPattern(Layout *self, unsigned img, int p3, int p4,
                                     int p5, int p6);                       // 0x74e00
extern "C" void PaintCanvas_DrawImage2D9(PaintCanvas *pc, int img, int x, int y,
                                         int w, int h, int a1, int a2, int a3); // 0x73564
extern "C" void PaintCanvas_DrawString(PaintCanvas *pc, void *font, void *str,
                                       int x, int y);                       // 0x6fe14
// Header transition tail helper @0x1ac0a8.
extern "C" void Layout_headerAnim(void *btn);

// Hidden globals from drawHeader disasm.
__attribute__((visibility("hidden"))) extern unsigned *g_dhColor;   // @0xe4118 ([0]=image/color)
__attribute__((visibility("hidden"))) extern int **g_dhMetric;      // @0xe41aa ([0][0x28],[0][0x44])
__attribute__((visibility("hidden"))) extern void ***g_dhFont;      // @0xe41ac

// Layout::drawHeader(String title, bool transition)
extern "C" void Layout_drawHeader7(Layout *self, void *title, int transition) {
    unsigned img = *g_dhColor;
    PaintCanvas_SetColor(F<unsigned>(self, 0x3b0));
    int iw = PaintCanvas_GetImage2DWidth(img);
    int ih = PaintCanvas_GetImage2DHeight(img);
    PaintCanvas_DrawImage2D3((PaintCanvas *)img, F<int>(self, 0x330),
                             F<int>(self, 0x2dc), F<int>(self, 0x2e0));
    Layout_drawBGPattern(self, F<unsigned>(self, 0x328),
                         F<int>(self, 0x2dc) + iw, F<int>(self, 0x2e0),
                         F<int>(self, 0x2e4) + iw * -2, ih);
    PaintCanvas_DrawImage2D9((PaintCanvas *)img, F<int>(self, 0x330),
                             F<int>(self, 0x2e4) + F<int>(self, 0x2dc),
                             F<int>(self, 0x2e0), iw, ih, 0x11, 0x12, 0x01);
    if (*(int *)((char *)title + 8) != 0) {
        PaintCanvas_DrawImage2D3((PaintCanvas *)img, F<int>(self, 0x32c),
                                 F<int>(self, 0x2dc), F<int>(self, 0x2e0));
        int *m = *g_dhMetric;
        PaintCanvas_DrawString((PaintCanvas *)img, **g_dhFont, title,
                               m[0x28 / 4] + m[0x44 / 4] + F<int>(self, 0x2dc),
                               F<int>(self, 0x18) + F<int>(self, 0x2e0));
    }
    F<uint8_t>(self, 0x3cc) = (uint8_t)transition;
    if (transition != 0 && F<uint8_t>(self, 0x0) == 0) {
        Layout_headerAnim(F<void *>(self, 0x3bc));
        return;
    }
}
