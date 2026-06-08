#include "class.h"

struct TouchButton;

// setRect-style helper invoked via fn ptr @0xe306c: (ptr, x, y, anchor).
extern "C" void Layout_setBtnRect(void *btn, int x, int y, int anchor);
extern "C" void TouchButton_getPosition(TouchButton *b);   // 0x74dd0 (writes s0/s1 stack)

// Hidden globals from resetWindowDimensions disasm.
__attribute__((visibility("hidden"))) extern int *g_rwGuard;     // @0xe305e (stack guard source [0])
__attribute__((visibility("hidden"))) extern int **g_rwW;        // @0xe3060 (** -> width)
__attribute__((visibility("hidden"))) extern int **g_rwH;        // @0xe3064 (** -> height)
__attribute__((visibility("hidden"))) extern int **g_rwMetric;   // @0xe3092 ([0][0x28] = inset)
__attribute__((visibility("hidden"))) extern int *g_rwOutX;      // @0xe30e8 (button x out)
__attribute__((visibility("hidden"))) extern int *g_rwOutY;      // @0xe3104 (button y out)

// Layout::resetWindowDimensions()
extern "C" void Layout_resetWindowDimensions(Layout *self) {
    int *guard = g_rwGuard;
    int g0 = *guard;
    int h = **g_rwH;
    int w = **g_rwW;

    F<int>(self, 0x2dc) = 0;
    F<int>(self, 0x2e0) = 0;
    F<int>(self, 0x2e4) = w;
    F<int>(self, 0x2e8) = h;

    Layout_setBtnRect(F<void *>(self, 0x3bc), w, 0, 0x12);

    int *m = *g_rwMetric;
    int inset = m[0x28 / 4];
    Layout_setBtnRect(F<void *>(self, 0x3b4),
                      inset + F<int>(self, 0x2dc),
                      (F<int>(self, 0x2e0) + F<int>(self, 0x2e8)) - F<int>(self, 0x3fc),
                      0x21);
    Layout_setBtnRect(F<void *>(self, 0x3b8),
                      inset + F<int>(self, 0x2dc),
                      (F<int>(self, 0x2e0) + F<int>(self, 0x2e8)) - F<int>(self, 0x3fc),
                      0x21);

    if (F<int>(self, 0x3b4) != 0) {
        float pos[2] __attribute__((aligned(4)));
        TouchButton_getPosition((TouchButton *)pos);
        *g_rwOutX = (int)pos[0];
        TouchButton_getPosition((TouchButton *)pos);
        *g_rwOutY = (int)pos[1];
    }

    if (*guard - g0 != 0)
        __stack_chk_fail();
}
