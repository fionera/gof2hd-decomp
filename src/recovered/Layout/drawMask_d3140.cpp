#include "class.h"

__attribute__((visibility("hidden"))) extern int *gW;  // ldr [0xe3160]
__attribute__((visibility("hidden"))) extern int *gH;  // ldr [0xe3164]
extern "C" void Layout_drawMaskImpl(void *pc, int a, int b, int w, int h);  // 0x74ddc

// Layout::drawMask() -> drawMask(*gH, 0, 0, *gW, *gH)
extern "C" void Layout_drawMask0() {
    void *pc = (void *)*gH;
    int w = *gW;
    Layout_drawMaskImpl(pc, 0, 0, w, *gH);
}
