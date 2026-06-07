#include "class.h"

extern "C" void Engine_SetGravValue(Engine *self, double x, double y, double z)
{
    *(double *)((char *)self + 0x4e0) = x;
    *(double *)((char *)self + 0x4e8) = y;
    *(double *)((char *)self + 0x4f0) = z;
}
