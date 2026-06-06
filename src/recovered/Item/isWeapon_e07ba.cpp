#define wchar_t gof2_wchar_t
#include "class.h"
#undef wchar_t

bool Item::isWeapon()
{
    return static_cast<uint32_t>(type) < 3;
}
