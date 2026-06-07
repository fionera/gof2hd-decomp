#include "class.h"

extern "C" void ScrollTouchBox_update(void *self, int dt);

void ScrollTouchWindow::update(int dt)
{
    return ScrollTouchBox_update(P(this, 0), dt);
}
