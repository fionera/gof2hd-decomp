#include "class.h"

struct PaintCanvas;

extern "C" void PaintCanvas_SetColor(unsigned color);                      // 0x6fac0
extern "C" void String_cstr_ctor(void *out, const char *s, bool);          // 0x6ee18
extern "C" void String_dtor(void *s);                                      // 0x6ee30
extern "C" void Layout_drawBox(PaintCanvas *pc, int p2, int x, int y,
                               int w, int h, void *str);                   // 0x7462c
extern "C" void PaintCanvas_DrawImage2D5(PaintCanvas *pc, int img, int x, int y,
                                         int anchor);                      // 0x71d70
extern "C" void Globals_drawLines(unsigned a, void *arr, int lines, int x, int y); // 0x74e9c

// Hidden globals from drawTip disasm.
__attribute__((visibility("hidden"))) extern int *g_dtGuard;     // @0xe4a26 (stack guard [0])
__attribute__((visibility("hidden"))) extern unsigned *g_dtColor;// @0xe4a3e ([0]=color)
__attribute__((visibility("hidden"))) extern int **g_dtMetricA;  // @0xe4a4e ([0][0x78],[0][4])
__attribute__((visibility("hidden"))) extern int *g_dtDimW;      // @0xe4a52 ([0]=width)
__attribute__((visibility("hidden"))) extern int *g_dtDimH;      // @0xe4a56 ([0]=height)
__attribute__((visibility("hidden"))) extern const char g_dtBoxLit[];   // @0xe4a74
__attribute__((visibility("hidden"))) extern void **g_dtLinesA;  // @0xe4ad0 ([0])
__attribute__((visibility("hidden"))) extern unsigned *g_dtLinesB;// @0xe4ad2 ([0])

// Layout::drawTip()
extern "C" void Layout_drawTip(Layout *self) {
    int *guard = g_dtGuard;
    int g0 = *guard;
    if (F<int>(self, 0x3c8) != 0) {
        PaintCanvas_SetColor(*g_dtColor);
        int *mA = *g_dtMetricA;
        int dimW = *g_dtDimW;
        int dimH = *g_dtDimH;
        int boxW = mA[0x78 / 4];

        unsigned char box[sizeof(String12)] __attribute__((aligned(4)));
        String_cstr_ctor(box, g_dtBoxLit, false);
        Layout_drawBox((PaintCanvas *)mA, 5,
                       (dimH >> 1) - (boxW >> 1), (dimW >> 1) + 0xd, boxW, 100, box);
        String_dtor(box);

        PaintCanvas_DrawImage2D5((PaintCanvas *)*g_dtColor, F<int>(self, 0x398),
                                 dimH >> 1, (dimW >> 1) + 0x3f, 0x11);

        int lineCount = *F<int *>(self, 0x3c8);
        int y = (dimW >> 1) + 0x3f - ((lineCount * mA[4 / 4]) >> 1);
        Globals_drawLines(*g_dtLinesB, *(void **)g_dtLinesA, F<int>(self, 0x3c8),
                          dimH >> 1, y);
    }
    if (*guard != g0)
        __stack_chk_fail();
}
