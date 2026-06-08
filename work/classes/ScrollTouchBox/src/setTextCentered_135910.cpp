#include "class.h"

void ScrollTouchBox::setTextCentered(bool centered)
{
    F<uint8_t>(this, 0x38) = centered;
}
