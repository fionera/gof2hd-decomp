#include "class.h"

extern "C" void Layout_drawFooterImpl(Layout *self, int a, int b);  // -> 0x1ac098

// Layout::drawFooter() -> drawFooter(this, 0, 1)
extern "C" void Layout_drawFooter(Layout *self) {
    return Layout_drawFooterImpl(self, 0, 1);
}
