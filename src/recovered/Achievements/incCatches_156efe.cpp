#include "class.h"

extern "C" void Achievements_incCatches(Achievements *self)
{
    F<int>(self, 0xc) += 1;
}
