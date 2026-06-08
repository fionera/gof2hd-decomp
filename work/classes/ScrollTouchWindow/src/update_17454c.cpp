#include "class.h"

extern "C" void ScrollTouchBox_update(void *self, int dt);

void ScrollTouchWindow::update(int dt)
{
    return ScrollTouchBox_update(this->f_0, dt);
}
