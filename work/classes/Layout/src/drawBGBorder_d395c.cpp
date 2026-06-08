#include "class.h"

struct PaintCanvas;

extern "C" int PaintCanvas_GetImage2DWidth(int img);                       // 0x72d84
extern "C" int PaintCanvas_GetImage2DHeight(int img);                      // 0x72d90
extern "C" void PaintCanvas_DrawImage2D3(PaintCanvas *pc, unsigned img, int x, int y); // 0x72dc0
extern "C" void PaintCanvas_DrawImage2D5(PaintCanvas *pc, unsigned img, int x,
                                         int y, int anchor);                // fn ptr @0xe39d2 / 0x74e3c
extern "C" int __aeabi_idiv(int a, int b);                                 // 0x7198c
extern "C" void PaintCanvas_DrawRegion2D(PaintCanvas *pc, unsigned img, int sx, int sy,
                                         int sw, int sh, int rot, int flip, int a, int b,
                                         int x, int y);                     // 0x72d9c

// Hidden globals from drawBGBorder disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_bbCanvas;  // @0xe3972
__attribute__((visibility("hidden"))) extern int g_bbFlipTR;            // @0xe3bf4 ([0])
__attribute__((visibility("hidden"))) extern int g_bbFlipL;             // @0xe3bf8 ([0])
__attribute__((visibility("hidden"))) extern int g_bbFlipR;             // @0xe3bfc ([0])

// Layout::drawBGBorder(uint corner, uint edge, int x, int y, int w, int h, int inset, int pad)
extern "C" void Layout_drawBGBorder(Layout *self, unsigned corner, unsigned edge,
                                    int x, int y, int w, int h, int inset, int pad) {
    PaintCanvas *pc = *g_bbCanvas;
    int cw = PaintCanvas_GetImage2DWidth(corner);
    int ch = PaintCanvas_GetImage2DHeight(corner);
    int ew = PaintCanvas_GetImage2DWidth(edge);
    int eh = PaintCanvas_GetImage2DHeight(edge);

    // Four corners (top-left at base; others mirrored via the 5-arg variant).
    PaintCanvas_DrawImage2D3(pc, corner, inset + x, inset + y);
    int rightX = ((w + x) - cw) - inset;
    PaintCanvas_DrawImage2D5(pc, corner, rightX, inset + y, 1);
    int bottomY = ((y + h) - ch) - inset;
    PaintCanvas_DrawImage2D5(pc, corner, inset + x, bottomY, 2);
    PaintCanvas_DrawImage2D5(pc, corner, rightX, bottomY, 3);

    // Top/bottom edges (horizontal tiling of `edge`).
    int spanW = w + cw * -2 + inset * -2;
    int colsH = __aeabi_idiv(spanW, ew);
    int baseX = pad + x;
    int tileX = cw + pad + x;
    int topRowY = pad + y;
    int botRowY = (h - inset) + pad + y;
    for (int i = 0; i < colsH; i++) {
        PaintCanvas_DrawImage2D3(pc, edge, tileX, topRowY);
        PaintCanvas_DrawImage2D5(pc, edge, tileX, botRowY, 2);
        tileX += ew;
    }
    int remW = spanW - colsH * ew;
    if (remW > 0) {
        PaintCanvas_DrawRegion2D(pc, edge, 0, 0, remW, eh, 0, 0, 0, 0,
                                 colsH * ew + baseX + cw, topRowY);
        PaintCanvas_DrawRegion2D(pc, edge, 0, 0, remW, eh, 0, g_bbFlipTR, 0, 0,
                                 spanW + baseX + cw, botRowY);
    }

    // Left/right edges (vertical tiling of `edge`).
    int spanH = h + ch * -2 + inset * -2;
    int rowsV = __aeabi_idiv(spanH, ew);
    int leftX = pad + x + inset;
    int rightEdgeX = w + x + pad;
    for (int i = 0; i < rowsV; i++) {
        PaintCanvas_DrawRegion2D(pc, edge, 0, 0, ew, eh, 0, g_bbFlipL, 0, 0, leftX, 0);
        PaintCanvas_DrawRegion2D(pc, edge, 0, 0, ew, eh, 0, g_bbFlipR, 0, 0, rightEdgeX, 0);
    }
    int remH = spanH - rowsV * ew;
    if (remH > 0) {
        PaintCanvas_DrawRegion2D(pc, edge, 0, 0, remH, eh, 0, g_bbFlipL, 0, 0, leftX, 0);
        PaintCanvas_DrawRegion2D(pc, edge, 0, 0, remH, eh, 0, g_bbFlipR, 0, 0,
                                 rightEdgeX + inset * -2, 0);
    }
}
