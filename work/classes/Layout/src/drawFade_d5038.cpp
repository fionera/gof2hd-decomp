#include "class.h"

struct PaintCanvas;

extern "C" unsigned PaintCanvas_GetColor(PaintCanvas *pc);   // 0x6fa90
extern "C" void PaintCanvas_SetColor(unsigned color);        // 0x6fac0
extern "C" void PaintCanvas_FillRectangle(PaintCanvas *pc, int x, int y, int wh); // 0x74de8

// Hidden globals from drawFade disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dfCanvasA;  // @0xe5052
__attribute__((visibility("hidden"))) extern int **g_dfDimA;             // @0xe50c8 ([0][0])
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dfCanvasB;  // @0xe50ee
__attribute__((visibility("hidden"))) extern int **g_dfDimB;             // @0xe5108 ([0][0])

// Layout::drawFade(): overlay the fade-in/out rectangles, returning the active flag.
extern "C" uint8_t Layout_drawFade(Layout *self) {
    if (F<uint8_t>(self, 0x400) != 0) {
        PaintCanvas *pc = *g_dfCanvasA;
        unsigned saved = PaintCanvas_GetColor(pc);

        // progress = duration / endTime  (fields 0x408 / 0x40c, signed->float)
        float t = (float)F<int>(self, 0x408) / (float)F<int>(self, 0x40c);
        if (F<uint8_t>(self, 0x401) != 0)
            t = 1.0f - t;
        if (t > 1.0f) t = 1.0f;

        unsigned color = F<unsigned>(self, 0x404);
        if (t > 0.0f)
            color += (int)(t * 255.0f);
        PaintCanvas_SetColor(color);
        PaintCanvas_FillRectangle(*g_dfCanvasA, 0, 0, **g_dfDimA);
        PaintCanvas_SetColor(saved);
    }
    if (F<uint8_t>(self, 0x410) != 0) {
        PaintCanvas *pc = *g_dfCanvasB;
        unsigned saved = PaintCanvas_GetColor(pc);
        PaintCanvas_SetColor(0xff);
        PaintCanvas_FillRectangle(*g_dfCanvasB, 0, 0, **g_dfDimB);
        PaintCanvas_SetColor(saved);
    }
    return F<uint8_t>(self, 0x400);
}
