#include "class.h"

struct PaintCanvas;

extern "C" unsigned PaintCanvas_GetColor(PaintCanvas *pc);                  // 0x6fa90
extern "C" void PaintCanvas_SetColor(unsigned color);                      // 0x6fac0
extern "C" int PaintCanvas_GetImage2DWidth(int img);                       // 0x72d84
extern "C" int PaintCanvas_GetImage2DHeight(int img);                      // 0x72d90
extern "C" void PaintCanvas_DrawImage2D3(PaintCanvas *pc, unsigned img, int x, int y); // 0x72dc0
extern "C" void PaintCanvas_DrawImage2D5(PaintCanvas *pc, unsigned img, int x,
                                         int y, int anchor);                // 0x74e3c
extern "C" void Layout_drawBGPattern(Layout *self, unsigned img, int p3, int p4,
                                     int p5, int p6);                       // 0x74e00
extern "C" void Layout_drawBGBorder8(Layout *self, unsigned a, unsigned b, int x,
                                     int y, int w, int h, int p8, int p9);  // 0x74e30
extern "C" void Layout_drawBGBorder6(Layout *self, unsigned a, int b, int x,
                                     int y, int w);                         // 0x74e78
extern "C" int PaintCanvas_GetTextWidth(unsigned pc, void *font);          // 0x6faa8
extern "C" void PaintCanvas_DrawString(unsigned pc, void *font, void *str,
                                       int x, int y);                       // 0x6fe14
// Color-restore tail helper @0x1ac088.
extern "C" void PaintCanvas_restoreColor(unsigned pc, unsigned saved);

// Hidden globals from drawBox disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dbCanvas;  // @0xe439c
__attribute__((visibility("hidden"))) extern int **g_dbMetric0;         // @0xe444a (case0 [0][0x44],[0][0x1c])
__attribute__((visibility("hidden"))) extern void **g_dbFont0c;         // @0xe446a
__attribute__((visibility("hidden"))) extern void **g_dbFont0r;         // @0xe47f2
__attribute__((visibility("hidden"))) extern void **g_dbFont0;          // @0xe480c
__attribute__((visibility("hidden"))) extern int **g_dbMetric1;         // @0xe44f6 (case1 [0][0x44],[0][0x5c])
__attribute__((visibility("hidden"))) extern void **g_dbFont1c;         // @0xe4516
__attribute__((visibility("hidden"))) extern void **g_dbFont1r;         // @0xe4830
__attribute__((visibility("hidden"))) extern void **g_dbFont1;          // @0xe484a
__attribute__((visibility("hidden"))) extern int **g_dbMetric6;         // @0xe463a (case6 [0][0x44])
__attribute__((visibility("hidden"))) extern void **g_dbFont6c;         // @0xe4656
__attribute__((visibility("hidden"))) extern void **g_dbFont6r;         // @0xe4874
__attribute__((visibility("hidden"))) extern void **g_dbFont6;          // @0xe4894
__attribute__((visibility("hidden"))) extern int **g_dbMetric7;         // @0xe467c (case7 [0][8],[0][0x28])
__attribute__((visibility("hidden"))) extern void **g_dbFont7;          // @0xe4708

// Layout::drawBox(int style, int x, int y, int w, int h, String text, uchar flags)
extern "C" void Layout_drawBox(Layout *self, int style, int x, int y, int w, int h,
                               void *text, unsigned flags) {
    PaintCanvas *pc = *g_dbCanvas;
    unsigned saved = PaintCanvas_GetColor(pc);
    PaintCanvas_SetColor(F<unsigned>(self, 0x3b0));

    switch (style) {
    case 0: {
        int iw = PaintCanvas_GetImage2DWidth(F<int>(self, 0x348));
        PaintCanvas_DrawImage2D3(pc, F<unsigned>(self, 0x348), x, y);
        Layout_drawBGPattern(self, F<unsigned>(self, 0x34c), iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, F<unsigned>(self, 0x348), (w + x) - iw, y, 0x01);
        if (*(int *)((char *)text + 8) == 0) break;
        int *mt = *g_dbMetric0;
        int tx = mt[0x44 / 4];
        if ((flags & 2) == 0) {
            if ((int)(flags << 0x1d) < 0) {
                int tw = PaintCanvas_GetTextWidth(*(unsigned *)g_dbCanvas, *g_dbFont0c);
                tx = w / 2 - tw / 2;
            }
        } else {
            int tw = PaintCanvas_GetTextWidth(*(unsigned *)g_dbCanvas, *g_dbFont0r);
            tx = (w - tx) - tw;
        }
        int ty = (y + (mt[0x1c / 4] >> 1) + 1) - F<int>(self, 0x3ac);
        PaintCanvas_DrawString(*(unsigned *)g_dbCanvas, *g_dbFont0, text, tx + x, ty);
        break;
    }
    case 1: {
        int iw = PaintCanvas_GetImage2DWidth(F<int>(self, 0x350));
        PaintCanvas_DrawImage2D3(pc, F<unsigned>(self, 0x350), x, y);
        Layout_drawBGPattern(self, F<unsigned>(self, 0x354), iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, F<unsigned>(self, 0x350), (w + x) - iw, y, 0x01);
        if (*(int *)((char *)text + 8) == 0) break;
        int *mt = *g_dbMetric1;
        int tx = mt[0x44 / 4];
        if ((flags & 2) == 0) {
            if ((int)(flags << 0x1d) < 0) {
                int tw = PaintCanvas_GetTextWidth(*(unsigned *)g_dbCanvas, *g_dbFont1c);
                tx = w / 2 - tw / 2;
            }
        } else {
            int tw = PaintCanvas_GetTextWidth(*(unsigned *)g_dbCanvas, *g_dbFont1r);
            tx = (w - tx) - tw;
        }
        int ty = (y + (mt[0x5c / 4] >> 1) + 1) - F<int>(self, 0x3ac);
        PaintCanvas_DrawString(*(unsigned *)g_dbCanvas, *g_dbFont1, text, tx + x, ty);
        break;
    }
    case 2:
        Layout_drawBGPattern(self, F<unsigned>(self, 0x324), x, y, w, h);
        break;
    case 3: {
        int iw = PaintCanvas_GetImage2DWidth(F<int>(self, 0x358));
        PaintCanvas_DrawImage2D3(pc, F<unsigned>(self, 0x358), x, y);
        Layout_drawBGPattern(self, F<unsigned>(self, 0x35c), iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, F<unsigned>(self, 0x358), (w + x) - iw, y, 0x01);
        break;
    }
    case 4: {
        int iw = PaintCanvas_GetImage2DWidth(F<int>(self, 0x36c));
        PaintCanvas_DrawImage2D3(pc, F<unsigned>(self, 0x36c), x, y);
        Layout_drawBGPattern(self, F<unsigned>(self, 0x370), iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, F<unsigned>(self, 0x36c), (w + x) - iw, y, 0x01);
        break;
    }
    case 5:
        Layout_drawBGBorder6(self, F<unsigned>(self, 0x380), F<int>(self, 0x384), x, y, w);
        break;
    case 6: {
        int iw = PaintCanvas_GetImage2DWidth(F<int>(self, 0x388));
        PaintCanvas_DrawImage2D3(pc, F<unsigned>(self, 0x388), x, y);
        Layout_drawBGPattern(self, F<unsigned>(self, 0x38c), iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, F<unsigned>(self, 0x388), (w + x) - iw, y, 0x01);
        if (*(int *)((char *)text + 8) == 0) break;
        int *mt = *g_dbMetric6;
        int tx = mt[0x44 / 4];
        if ((flags & 2) == 0) {
            if ((int)(flags << 0x1d) < 0) {
                int tw = PaintCanvas_GetTextWidth(*(unsigned *)g_dbCanvas, *g_dbFont6c);
                tx = w / 2 - tw / 2;
            }
        } else {
            int tw = PaintCanvas_GetTextWidth(*(unsigned *)g_dbCanvas, *g_dbFont6r);
            tx = (w - tx) - tw;
        }
        int ty = (y + (h >> 1) + 1) - F<int>(self, 0x3ac);
        PaintCanvas_DrawString(*(unsigned *)g_dbCanvas, *g_dbFont6, text, tx + x, ty);
        break;
    }
    case 7: {
        int *mt = *g_dbMetric7;
        int hdr = mt[8 / 4];
        Layout_drawBGPattern(self, F<unsigned>(self, 0x324), x, hdr + y, w, h - hdr);
        int ih = PaintCanvas_GetImage2DHeight(F<int>(self, 0x394));
        Layout_drawBGBorder8(self, F<unsigned>(self, 0x390), F<unsigned>(self, 0x394),
                             x, hdr + y, w, h - hdr, -ih, -ih);
        if (*(int *)((char *)text + 8) == 0) break;
        PaintCanvas_SetColor(0xffffffff);
        PaintCanvas_DrawImage2D3(pc, F<unsigned>(self, 0x32c), x, y);
        int ty = (y + (mt[8 / 4] / 2) + 1) - F<int>(self, 0x3ac);
        PaintCanvas_DrawString(*(unsigned *)g_dbCanvas, *g_dbFont7, text,
                               mt[0x28 / 4] + x, ty);
        break;
    }
    case 8:
        Layout_drawBGBorder6(self, F<unsigned>(self, 0x39c), F<int>(self, 0x3a0), x, y, w);
        break;
    case 9: {
        int iw = PaintCanvas_GetImage2DWidth(F<int>(self, 0x360));
        PaintCanvas_DrawImage2D3(pc, F<unsigned>(self, 0x360), x, y);
        Layout_drawBGPattern(self, F<unsigned>(self, 0x364), iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, F<unsigned>(self, 0x360), (w + x) - iw, y, 0x01);
        break;
    }
    case 10: {
        int iw = PaintCanvas_GetImage2DWidth(F<int>(self, 0x368));
        PaintCanvas_DrawImage2D3(pc, F<unsigned>(self, 0x368), x, y);
        Layout_drawBGPattern(self, F<unsigned>(self, 0x370), iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, F<unsigned>(self, 0x368), (w + x) - iw, y, 0x01);
        break;
    }
    default:
        break;
    }

    PaintCanvas_restoreColor(*(unsigned *)g_dbCanvas, saved);
}
