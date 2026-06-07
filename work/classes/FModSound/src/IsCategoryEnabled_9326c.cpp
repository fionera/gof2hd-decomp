#include "class.h"

// FModSound::IsCategoryEnabled(int) -> p1<=3 && system && enabled[p1] ? 1 : 0
struct FModSound { uint8_t IsCategoryEnabled(int p1); };
uint8_t FModSound::IsCategoryEnabled(int p1)
{
    uint8_t r = 0;
    if (p1 <= 3 && u32(this, OFF_SYSTEM) != 0 && u8(this, p1 + OFF_ENABLED) != 0)
        r = 1;
    return r;
}
