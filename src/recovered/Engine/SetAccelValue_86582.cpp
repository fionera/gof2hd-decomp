#include "class.h"

extern "C" void Engine_SetAccelValue(Engine *self, double x, double y, double z)
{
    *(double *)((char *)self + 0x4b0) = x;
    *(double *)((char *)self + 0x4b8) = y;
    *(double *)((char *)self + 0x4c0) = z;
}
