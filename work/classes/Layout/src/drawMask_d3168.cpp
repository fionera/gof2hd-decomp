#include "class.h"

__attribute__((visibility("hidden"))) extern void **gPC;          // ldr [0xe31ac]
extern "C" int PaintCanvas_GetColor(void *pc);                     // 0x6fa90
extern "C" void PaintCanvas_SetColor(void *pc, int color);         // 0x6fac0
extern "C" void PaintCanvas_FillRectangle(void *pc, int a, int b, int c); // 0x74de8
extern "C" int Layout_drawMaskTail(void *pc, int color, int p4, void *st); // tail 0x1ac088

// Layout::drawMask(int p1, int p2, int p3, int p4) — singleton reloaded each call.
extern "C" int Layout_drawMask4(Layout *self, int p1, int p2, int p3, int p4) {
    int saved = PaintCanvas_GetColor(*gPC);
    PaintCanvas_SetColor(*gPC, 0x80);
    PaintCanvas_FillRectangle(*gPC, p1, p2, p3);
    return Layout_drawMaskTail(*gPC, saved, p4, (void *)0);
}
