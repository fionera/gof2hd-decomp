#include "class.h"

extern "C" uint8_t Explosion_isPlaying(Explosion *self)
{
    return UC(self, 0x20);
}
