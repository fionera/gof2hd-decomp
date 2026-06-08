#include "class.h"

struct PaintCanvas;
struct TouchButton;

extern "C" void PaintCanvas_SetColor(unsigned color);                       // 0x6fac0
extern "C" int PaintCanvas_GetImage2DWidth(int img);                        // 0x72d84
extern "C" void PaintCanvas_DrawImage2D(PaintCanvas *pc, unsigned img, int x,
                                        int y, int anchor);                 // 0x71d70
extern "C" void Layout_drawBGPattern(Layout *self, unsigned img, int p3, int p4,
                                     int p5, int p6);                       // 0x74e00
extern "C" void TouchButton_setVisible(TouchButton *b, int v);             // 0x74e48
// Footer "scroll into place" tail-called helper at 0x1ac0a8.
extern "C" void TouchButton_footerAnim(TouchButton *b, int one, int h, void *sp);

// Hidden globals from drawEmptyFooter disasm.
__attribute__((visibility("hidden"))) extern unsigned *g_efColor;   // @0xe3fd2 ([0]=color)
__attribute__((visibility("hidden"))) extern int *g_efScreenH;      // @0xe3ff8 ([0]=screen height)
__attribute__((visibility("hidden"))) extern int *g_efScreenW;      // @0xe4016 ([0]=screen width)
__attribute__((visibility("hidden"))) extern int **g_efMetric;      // @0xe401c ([0][0x10]=footer height)

// Layout::drawEmptyFooter(bool showBack)
extern "C" void Layout_drawEmptyFooter(Layout *self, int showBack) {
    unsigned color = *g_efColor;
    PaintCanvas_SetColor(color);
    int w = PaintCanvas_GetImage2DWidth(color);
    int screenH = *g_efScreenH;
    PaintCanvas_DrawImage2D((PaintCanvas *)color, F<unsigned>(self, 0x334), 0, screenH, 0x11);

    int screenW = *g_efScreenW;
    int footerH = (*g_efMetric)[0x10 / 4];
    Layout_drawBGPattern(self, F<unsigned>(self, 0x344), w, screenH - footerH,
                         screenW - w * 2, footerH);
    PaintCanvas_DrawImage2D((PaintCanvas *)color, F<unsigned>(self, 0x334),
                            screenW - w, screenH - (*g_efMetric)[0x10 / 4], 0x01);
    if (showBack == 0) return;
    TouchButton_setVisible(F<TouchButton *>(self, 0x3b4), 1);
    unsigned char sp[8] __attribute__((aligned(4)));
    TouchButton_footerAnim(F<TouchButton *>(self, 0x3b4), 1, footerH, sp);
}
