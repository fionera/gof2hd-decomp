#include "class.h"

extern "C" void Gun_setErrorMagnitudePercentage(Gun *self, int v)
{
    self->f_fc = (float)v;
}
