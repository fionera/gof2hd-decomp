#include "class.h"

extern "C" bool Station_equals(Station *self, Station *other)
{
    if (other != 0)
        return self->f_c == other->f_c;
    return false;
}
