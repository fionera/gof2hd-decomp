#include "class.h"

extern "C" int PaintCanvas_GetTextWidth(int canvas, void *font);
extern "C" void PaintCanvas_DrawString(int canvas, void *font, int str, int x, int y,
                                       int flag);
extern void *const gDL_canvas __attribute__((visibility("hidden")));
extern void *const gDL_lineHeight __attribute__((visibility("hidden")));

// Globals::drawLines(uint, Array<String*>*, int, int, bool) — 5-decl form; the body
// consumes 6 incoming values: p1(unused), font(r1), lines(r2), baseX(r3), startY(stack0),
// centered(stack1).
extern "C" void Globals_drawLines5(unsigned p1, void *font, Array<int> *lines, int baseX,
                                   int startY, int centered)
{
    (void)p1;
    int *cv = (int *)gDL_canvas;          // global value (pointer); deref'd each iteration
    int **lh = (int **)gDL_lineHeight;    // global value (pointer)
    int yacc = startY;
    int dx = 0;
    for (unsigned i = 0; i < lines->length; i++) {
        if (centered != 0) {
            int w = PaintCanvas_GetTextWidth(*cv, font);
            dx = -(w >> 1);
        }
        PaintCanvas_DrawString(*cv, font, lines->data[i], dx + baseX, yacc, 0);
        yacc += *(int *)((char *)*lh + 4);
    }
}
