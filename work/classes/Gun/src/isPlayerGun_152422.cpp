#include "class.h"

extern "C" uint8_t Gun_isPlayerGun(Gun *self)
{
    return self->f_f0;
}
