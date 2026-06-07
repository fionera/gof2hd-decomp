#include "class.h"

extern "C" void Gun_removeAllEnemies(Gun *self)
{
    F<int>(self, 0xb4) = 0;
}
