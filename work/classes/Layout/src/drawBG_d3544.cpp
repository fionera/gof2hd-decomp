#include "class.h"

__attribute__((visibility("hidden"))) extern int *gA;  // ldr [0xe356c]
__attribute__((visibility("hidden"))) extern int *gB;  // ldr [0xe3570]
extern "C" void Layout_drawBGPattern(Layout *self, unsigned f324, int a, int b, int p4, int p5); // 0x74e00

// Layout::drawBG() -> drawBGPattern(this, this->0x324, 0, 0, *gA, *gB)
extern "C" void Layout_drawBG(Layout *self) {
    int p4 = *gA;
    int p5 = *gB;
    Layout_drawBGPattern(self, F<unsigned>(self, 0x324), 0, 0, p4, p5);
}
