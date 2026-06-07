#include "class.h"

extern "C" void Layout_drawFooterImpl(Layout *self, int a, int b);

// Layout::drawFooterStation() -> drawFooter(this, 1, 0)  (this passed implicitly in r0)
extern "C" void Layout_drawFooterStation(Layout *self) {
    return Layout_drawFooterImpl(self, 1, 0);
}
