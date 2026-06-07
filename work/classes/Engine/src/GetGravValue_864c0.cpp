#include "class.h"

extern "C" double *Engine_GetGravValue(Engine *self)
{
    double x = *(double *)((char *)self + 0x4e0);
    double y;
    if (*(int *)(**(char ***)((char *)self + 0x30) + 0x30) == 1) {
        x = -x;
        y = -*(double *)((char *)self + 0x4e8);
    } else {
        y = *(double *)((char *)self + 0x4e8);
    }
    *(volatile double *)((char *)self + 0x4f8) = x;
    *(volatile double *)((char *)self + 0x500) = y;
    *(double *)((char *)self + 0x508) = *(volatile double *)((char *)self + 0x4f0);
    return (double *)((char *)self + 0x4f8);
}
