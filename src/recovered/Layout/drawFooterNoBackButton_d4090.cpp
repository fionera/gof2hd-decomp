#include "class.h"

extern "C" void Layout_drawFooterImpl(Layout *self, int a, int b);

// Layout::drawFooterNoBackButton() -> drawFooter(this, 0, 0)
extern "C" void Layout_drawFooterNoBackButton(Layout *self) {
    return Layout_drawFooterImpl(self, 0, 0);
}
