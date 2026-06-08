#include "class.h"

extern "C" void ScrollTouchWindow_update(void *self, int dt);

void ChoiceWindow::update(int dt)
{
    ScrollTouchWindow_update(F<void *>(this, 0x1c), dt);
}
