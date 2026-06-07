#include "class.h"

extern "C" void ScrollTouchBox_OnTouchEnd(void *self, int x, int y);

void ScrollTouchWindow::OnTouchEnd(int x, int y)
{
    ScrollTouchBox_OnTouchEnd(P(this, 0), x, y);
    UC(this, 0x10) = 0;
}
