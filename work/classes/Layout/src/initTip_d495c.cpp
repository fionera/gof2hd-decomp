#include "class.h"

struct PaintCanvas;

extern "C" void *operator_new(unsigned sz);
extern "C" void operator_delete(void *p);
extern "C" void ArrayReleaseClasses_StringPtr(void *arr);    // 0x6facc
extern "C" void *ArrayStringPtr_dtor(void *arr);             // 0x6f64c
extern "C" void ArrayStringPtr_ctor(void *arr);             // 0x6f628
extern "C" int AERandom_nextInt(int n);                     // 0x71848
extern "C" void *GameText_getText(int id);                  // 0x72f70
extern "C" void PaintCanvas_GetLineArray(PaintCanvas *pc, unsigned color, void *str,
                                         int width, void *outArr);  // 0x74e90

// Hidden PC-relative globals from initTip disasm:
//   g_tipColor  : *(uint**) @0xe499c — color value (read [0]).
//   g_tipTextId : *(int**)  @0xe49a0 — text-table id (read **).
//   g_tipRandN  : *(int**)  @0xe49a2 — random bound (read **).
//   g_tipCanvas : *(PaintCanvas***) @0xe49a6 — canvas (read **).
//   g_tipMetric : *(int**)  @0xe49d4 — object whose [0x78]/[0x4c] give width.
__attribute__((visibility("hidden"))) extern unsigned *g_tipColor;
__attribute__((visibility("hidden"))) extern int **g_tipTextId;
__attribute__((visibility("hidden"))) extern int **g_tipRandN;
__attribute__((visibility("hidden"))) extern PaintCanvas ***g_tipCanvas;
__attribute__((visibility("hidden"))) extern int **g_tipMetric;

// Layout::initTip(): rebuild the random "tip" string line array.
extern "C" void Layout_initTip(Layout *self) {
    void **tipArr = (void **)((char *)self + 0x3c8);
    if (*tipArr != 0) {
        ArrayReleaseClasses_StringPtr(*tipArr);
        if (*tipArr != 0)
            operator_delete(ArrayStringPtr_dtor(*tipArr));
        *tipArr = 0;
    }
    void *arr = operator_new(0xc);
    ArrayStringPtr_ctor(arr);

    unsigned color = *g_tipColor;
    int textId = **g_tipTextId;
    PaintCanvas *canvas = **g_tipCanvas;
    AERandom_nextInt(**g_tipRandN);
    void *str = GameText_getText(textId);

    *tipArr = arr;
    int *m = *g_tipMetric;
    int width = m[0x78 / 4] + m[0x4c / 4] * -2;
    PaintCanvas_GetLineArray(canvas, color, str, width, *tipArr);
}
