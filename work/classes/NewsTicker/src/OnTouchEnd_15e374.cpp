#include "class.h"

uint32_t NewsTicker::OnTouchEnd(int, int)
{
    if (F<uint8_t>(this, 0x28) == 0) {
        return 0;
    }
    F<uint8_t>(this, 0x28) = 0;
    return 1;
}
