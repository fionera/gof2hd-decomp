#include "class.h"

extern "C" uint8_t Gun_isPlayerGun(Gun *self)
{
    return F<uint8_t>(self, 0xf0);
}
