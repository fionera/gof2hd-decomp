#include "class.h"

extern "C" void ScrollTouchBox_OnTouchBegin(void *self, int x, int y);

void ScrollTouchWindow::OnTouchBegin(int x, int y)
{
    return ScrollTouchBox_OnTouchBegin(this->f_0, x, y);
}
