#include "class.h"

extern "C" int Gun_getMagnitude(Gun *self)
{
    return F<int>(self, 0x100);
}
