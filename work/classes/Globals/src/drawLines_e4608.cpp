#include "class.h"

extern "C" int PaintCanvas_GetTextWidth(int canvas, void *font);
extern "C" void PaintCanvas_DrawString(int canvas, void *font, int str, int x, int y,
                                       int flag);
extern void *const gDL2_canvas __attribute__((visibility("hidden")));     // DAT_000f4680
extern void *const gDL2_lineHeight __attribute__((visibility("hidden"))); // DAT_000f4684

// Globals::drawLines(uint, Array<String*>*, int, int, uint, bool) — 7-decl form.
// param_1(font), lines(r2), baseX(r3), startY(stack r7+0xc), rightX(param_5 r7+...), centered(stack).
// When NOT centered: dx = rightX - GetTextWidth(); when centered: dx stays 0.
extern "C" void Globals_drawLines7(unsigned font, Array<int> *lines, int baseX, int startY,
                                   unsigned rightX, int centered)
{
    int *cv = (int *)gDL2_canvas;          // global value (pointer); deref'd each iteration
    int **lh = (int **)gDL2_lineHeight;    // global value (pointer)
    int yacc = startY;
    int dx = 0;
    for (unsigned i = 0; i < lines->length; i++) {
        if (centered == 0) {
            int w = PaintCanvas_GetTextWidth(*cv, (void *)font);
            dx = (int)rightX - w;
        }
        PaintCanvas_DrawString(*cv, (void *)font, lines->data[i], dx + baseX, yacc, 0);
        yacc += *(int *)((char *)*lh + 4);
    }
}
