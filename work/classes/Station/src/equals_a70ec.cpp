#include "class.h"

extern "C" bool Station_equals(Station *self, Station *other)
{
    if (other != 0)
        return F<int>(self, 0xc) == F<int>(other, 0xc);
    return false;
}
