#include "class.h"

extern "C" void Gun_setErrorMagnitudePercentage(Gun *self, int v)
{
    F<float>(self, 0xfc) = (float)v;
}
