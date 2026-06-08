#include "class.h"

struct PaintCanvas;

extern "C" unsigned PaintCanvas_GetColor(PaintCanvas *pc);                  // 0x6fa90
extern "C" void PaintCanvas_SetColor(unsigned color);                      // 0x6fac0
extern "C" void Layout_drawBGPattern(Layout *self, unsigned img, int p3, int p4,
                                     int p5, int p6);                       // 0x74e00
extern "C" void PaintCanvas_DrawRectangle(PaintCanvas *pc, int x, int y,
                                          int w, int h);                    // 0x74e18
extern "C" void PaintCanvas_DrawImage2D3(PaintCanvas *pc, int img, int x);  // 0x72dc0
extern "C" int AbyssString_Compare(const void *s, const char *lit);        // 0x6fa3c
extern "C" void PaintCanvas_DrawString(PaintCanvas *pc, void *font, void *str,
                                       int x, int y);                       // 0x6fe14
// Color-restore tail helper @0x1ac088.
extern "C" void PaintCanvas_restoreColor(PaintCanvas *pc, unsigned saved);

// Hidden globals from drawWindow disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dwCanvas;  // @0xe3796
__attribute__((visibility("hidden"))) extern int **g_dwBorderTop;       // @0xe37b0 ([0][8])
__attribute__((visibility("hidden"))) extern int **g_dwMetric;          // @0xe37d8 ([0][8],[0][0x28])
__attribute__((visibility("hidden"))) extern const char g_dwCmpLit[];   // @0xe381a
__attribute__((visibility("hidden"))) extern void ***g_dwFont;          // @0xe382c

// Layout::drawWindow(String, int x, int y, int w, int h, bool drawBG)
extern "C" void Layout_drawWindow7(Layout *self, void *title, int x, int y,
                                   int w, int h, int drawBG) {
    PaintCanvas *pc = *g_dwCanvas;
    unsigned saved = PaintCanvas_GetColor(pc);
    if (drawBG != 0) {
        int top = (*g_dwBorderTop)[8 / 4];
        Layout_drawBGPattern(self, F<unsigned>(self, 0x324), x, top + y, w, h - top);
    }
    PaintCanvas_SetColor(*(unsigned *)g_dwCanvas);  // *puVar3 (the color int slot)
    int *m = *g_dwMetric;
    int top = m[8 / 4];
    PaintCanvas_DrawRectangle(*g_dwCanvas, x, top + y, w, h - top);
    PaintCanvas_SetColor(F<unsigned>(self, 0x3b0));
    PaintCanvas_DrawImage2D3(*g_dwCanvas, F<int>(self, 0x32c), x);
    if (*(int *)((char *)title + 8) != 0 &&
        AbyssString_Compare(title, g_dwCmpLit) == 0) {
        int *mm = *g_dwMetric;
        int half = mm[8 / 4];
        half += half >> 31;
        int ty = (y + (half >> 1) + 1) - F<int>(self, 0x3ac);
        PaintCanvas_DrawString(*g_dwCanvas, **g_dwFont, title, mm[0x28 / 4] + x, ty);
    }
    PaintCanvas_restoreColor(*g_dwCanvas, saved);
}
