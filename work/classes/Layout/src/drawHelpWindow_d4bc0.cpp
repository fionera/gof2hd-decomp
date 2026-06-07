#include "class.h"

extern "C" void Layout_drawHelpWindowImpl(void *p);  // -> 0x1ac0b8

// Layout::drawHelpWindow() -> ext(*(this+0x3c4))
extern "C" void Layout_drawHelpWindow(Layout *self) {
    return Layout_drawHelpWindowImpl(F<void *>(self, 0x3c4));
}
