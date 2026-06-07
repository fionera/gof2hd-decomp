#include "class.h"

extern "C" void ScrollTouchBox_setTextCentered(void *self, bool centered);

void ScrollTouchWindow::setTextCentered(bool centered)
{
    return ScrollTouchBox_setTextCentered(P(this, 0), centered);
}
