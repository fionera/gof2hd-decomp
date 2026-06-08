#include "class.h"

extern "C" void *Gun_getEnemies(Gun *self)
{
    return self->f_b4;
}
