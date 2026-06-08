#include "class.h"

uint32_t NewsTicker::OnTouchEnd(int, int)
{
    if (this->f_28 == 0) {
        return 0;
    }
    this->f_28 = 0;
    return 1;
}
