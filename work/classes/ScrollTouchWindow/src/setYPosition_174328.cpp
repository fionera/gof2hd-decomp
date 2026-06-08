#include "class.h"

extern "C" void ScrollTouchBox_setYPosition(void *self, int y);

void ScrollTouchWindow::setYPosition(int y)
{
    return ScrollTouchBox_setYPosition(this->f_0, y);
}
