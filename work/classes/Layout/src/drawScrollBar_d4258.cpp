#include "class.h"

struct PaintCanvas;

extern "C" int PaintCanvas_GetImage2DWidth(int img);                       // 0x72d84
extern "C" int PaintCanvas_GetImage2DHeight(int img);                      // 0x72d90
extern "C" void PaintCanvas_SetColor(unsigned color);                      // fn ptr @0xe428e
extern "C" void PaintCanvas_DrawRectangle(PaintCanvas *pc, int x, int y,
                                          int w, int h);                    // 0x74e18
extern "C" void Layout_drawBGPattern(Layout *self, unsigned img, int p3, int p4,
                                     int p5, int p6);                       // 0x74e00
extern "C" void PaintCanvas_DrawImage2D3(PaintCanvas *pc, int img, int x);  // 0x72dc0
extern "C" void PaintCanvas_DrawImage2D5(PaintCanvas *pc, unsigned img, int x,
                                         int y, int anchor);                // 0x74e3c

// Hidden globals from drawScrollBar disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_sbCanvas;  // @0xe426e
__attribute__((visibility("hidden"))) extern unsigned g_sbColor0;       // @0xe4370 ([0])
__attribute__((visibility("hidden"))) extern unsigned g_sbColor1;       // @0xe4374 ([0])
__attribute__((visibility("hidden"))) extern int **g_sbMetric;          // @0xe42a2 ([0][0x48])

// Layout::drawScrollBar(int x, int y, int trackH, int pos, int range)
extern "C" void Layout_drawScrollBar(Layout *self, int x, int y, int trackH,
                                     int pos, int range) {
    PaintCanvas *pc = *g_sbCanvas;
    int iw = PaintCanvas_GetImage2DWidth(F<int>(self, 0x374));
    int ih = PaintCanvas_GetImage2DHeight(F<int>(self, 0x374));

    PaintCanvas_SetColor(*(unsigned *)&g_sbColor0);
    PaintCanvas_SetColor(*(unsigned *)&g_sbColor1);

    int inset = F<int>(self, 0x3e0);
    PaintCanvas_DrawRectangle(pc, x, inset + y, (*g_sbMetric)[0x48 / 4], trackH - inset * 2);
    PaintCanvas_SetColor(F<unsigned>(self, 0x3b0));

    int thumb = range - 1;
    if (thumb <= ih * 2) thumb = ih * 2;
    int off = pos + 1;
    if (trackH <= thumb + off)
        off = (trackH - thumb) - inset;

    int handle = F<int>(self, 0x3e4);
    thumb = thumb + handle * -4;
    off = off + handle * 2;
    if (ih * 2 < thumb) {
        Layout_drawBGPattern(self, F<unsigned>(self, 0x378),
                             x + 1 + handle, ih + y + off, iw, thumb + ih * -2);
        handle = F<int>(self, 0x3e4);
    } else {
        int lim = trackH + ih * -2;
        if (lim <= off) off = lim;
    }

    PaintCanvas_DrawImage2D3(pc, F<int>(self, 0x374), handle + x + 1);
    PaintCanvas_DrawImage2D5(pc, F<unsigned>(self, 0x374),
                             F<int>(self, 0x3e4) + x + 1, (thumb - ih) + y + off, 0x02);
}
