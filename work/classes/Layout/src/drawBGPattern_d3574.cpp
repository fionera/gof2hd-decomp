#include "class.h"

struct PaintCanvas;

extern "C" void PaintCanvas_SetColor(unsigned color);                      // 0x6fac0
extern "C" int PaintCanvas_GetImage2DWidth(int img);                       // 0x72d84
extern "C" int PaintCanvas_GetImage2DHeight(int img);                      // 0x72d90
extern "C" int __aeabi_idiv(int a, int b);                                 // 0x7198c
extern "C" void PaintCanvas_DrawImage2D3(PaintCanvas *pc, unsigned img, int x, int y); // 0x72dc0
extern "C" void PaintCanvas_DrawRegion2D(PaintCanvas *pc, unsigned img, int sx, int sy,
                                         int sw, int sh, int rot, int a, int b, int c,
                                         int x, int y);                     // 0x72d9c

// Hidden global from drawBGPattern disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_bgCanvas;  // @0xe358a

// Layout::drawBGPattern(uint img, int x, int y, int w, int h): tile `img` to fill the area.
extern "C" void Layout_drawBGPattern(Layout *self, unsigned img, int x, int y,
                                     int w, int h) {
    PaintCanvas *pc = *g_bgCanvas;
    PaintCanvas_SetColor(F<unsigned>(self, 0x3b0));
    int iw = PaintCanvas_GetImage2DWidth(img);
    int cols = __aeabi_idiv(w, iw);
    int fullW = cols * iw;
    int ih = PaintCanvas_GetImage2DHeight(img);
    int rows = __aeabi_idiv(h, ih);
    int fullH = rows * ih;

    for (int r = 0; r < rows; r++) {
        int py = y + r * ih;
        int px = x;
        for (int c = 0; c < cols; c++) {
            PaintCanvas_DrawImage2D3(pc, img, px, py);
            px += iw;
        }
    }

    int remH = h - fullH;
    int remW = w - fullW;
    if (remW > 0) {
        int py = y + fullH;
        for (int r = 0; r < rows; r++) {
            PaintCanvas_DrawRegion2D(pc, img, 0, 0, remW, ih, 0, 0, 0, 0, x + fullW, py);
            py += ih;
        }
    }
    if (remH > 0) {
        int px = x + fullW;
        (void)px;
        int px2 = x;
        for (int c = 0; c < cols; c++) {
            PaintCanvas_DrawRegion2D(pc, img, 0, 0, iw, remH, 0, 0, 0, 0, px2, y + fullH);
            px2 += iw;
        }
    }
    if (remW > 0 || remH > 0) {
        PaintCanvas_DrawRegion2D(pc, img, 0, 0, remW, remH, 0, 0, 0, 0,
                                 x + fullW, y + fullH);
    }
}
