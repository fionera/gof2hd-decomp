#include "class.h"

extern "C" int Gun_getMagnitude(Gun *self)
{
    return self->f_100;
}
