#include "class.h"

extern "C" void ScrollTouchBox_OnTouchMove(void *self, int x, int y);

void ScrollTouchWindow::OnTouchMove(int x, int y)
{
    ScrollTouchBox_OnTouchMove(P(this, 0), x, y);
    UC(this, 0x10) = 1;
}
