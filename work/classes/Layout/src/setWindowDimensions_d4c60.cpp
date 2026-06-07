#include "class.h"

// Function pointer (TouchButton::setPosition) loaded from a global, called via blx.
typedef void (*SetPosFn)(void *btn, int x, int y, int mode);
__attribute__((visibility("hidden"))) extern SetPosFn gSetPos;   // ldr [0xe4d30]
struct TB; struct TBHolder { TB *o; };
__attribute__((visibility("hidden"))) extern int **gTB;          // ldr [0xe4d34]
extern "C" void TouchButton_getPosition(void *btn);              // returns via regs
__attribute__((visibility("hidden"))) extern int *gPosX;         // ldr [0xe4d38]
__attribute__((visibility("hidden"))) extern int *gPosY;         // ldr [0xe4d3c]

// Layout::setWindowDimensions(int p1, int p2, int p3, int p4)
extern "C" void Layout_setWindowDimensions(Layout *self, int p1, int p2, int p3, int p4) {
    SetPosFn setPos = gSetPos;
    F<int>(self, 0x2dc) = p1;
    F<int>(self, 0x2e0) = p2;
    F<int>(self, 0x2e4) = p3;
    F<int>(self, 0x2e8) = p4;
    setPos(F<void *>(self, 0x3bc), p3 + p1, p2, 0x12);
    int *tb = *gTB;
    setPos(F<void *>(self, 0x3b4), tb[0x28 / 4] + F<int>(self, 0x2dc),
           (F<int>(self, 0x2e0) + F<int>(self, 0x2e8)) - F<int>(self, 0x3fc), 0x21);
    setPos(F<void *>(self, 0x3b8), tb[0x28 / 4] + F<int>(self, 0x2dc),
           (F<int>(self, 0x2e0) + F<int>(self, 0x2e8)) - F<int>(self, 0x3fc), 0x21);
}
