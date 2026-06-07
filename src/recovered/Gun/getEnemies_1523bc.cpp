#include "class.h"

extern "C" void *Gun_getEnemies(Gun *self)
{
    return F<void *>(self, 0xb4);
}
